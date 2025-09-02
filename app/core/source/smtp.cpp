#include "pch.h"
#include "smtp.h"
#include <curl/curl.h>
#include <string.h>
#include <stdio.h>
#include <thread>
#include "utils.h"
namespace SMTP
{
    static std::string EmailMsg(const std::vector<std::string>& toList, const std::string& from,const std::string& subject, const std::string& body)
    {
        std::string id = "<" + std::to_string(rand() % INT_MAX) + "@gmail.com>"; // unique email id

        std::string toCombined; // combined email list
        for (size_t i = 0; i < toList.size(); i++)
        {
            toCombined += toList[i];
            if (i < toList.size() - 1)
                toCombined += ", ";
        }
        std::string message =
            "Date: Mon, 29 Nov 2010 21:54:29 +1100\r\n"
            "From: " + from + "\r\n"
            "Message-ID: " + id + "\r\n"
            "Subject: " + subject + "\r\n"
            "Mime-Version: 1.0\r\n"
            "Content-Type: text/plain; charset=UTF-8\r\n"
            "Content-Transfer-Encoding: 7bit\r\n"
            "User-Agent: MyMailClient/1.0\r\n"
            "\r\n" +
            body + "\r\n";

        return message;
    }

    struct Context
    {
        size_t readed;
        std::string payload;
    };

    static size_t PayloadSrc(char* ptr, size_t size, size_t nmemb, void* userp)
    {
        if (!ptr || !userp)
            return 0;

        size_t room = size * nmemb;
        if (room == 0)
            return 0;

        Context* ctx = (Context*)userp;

        size_t remaining = ctx->payload.size() - ctx->readed;
        size_t len = (remaining < room) ? remaining : room;

        memcpy(ptr, ctx->payload.data() + ctx->readed, len);
        ctx->readed += len;

        return len;
    }

    using guard = std::lock_guard <std::mutex>;
    ///////////////////
    Request::Request(const Entity::User& sender, const std::string& smtpAddr)
    {
        this->sender = sender;
        this->smtpAddr = smtpAddr;
    }
    void Request::SetSender(const Entity::User& sender)
    {
        guard lock(mx);
        this->sender = sender;
    }
    void Request::SetServer(const std::string& smtpAddr)
    {
        guard lock(mx);
        this->smtpAddr = smtpAddr;
    }
    bool Request::IsValidSmtp()
    {
        guard lock(mx);
        std::regex pattern(R"(smtps?:\/\/[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
        return std::regex_search(smtpAddr, pattern);
    }
    void Request::Send(const std::vector<std::string>& receiversEmail, const std::string& subject, const std::string& body) const
    {
        CURL* curl;
        CURLcode res = CURLE_OK;
        struct curl_slist* recipients = nullptr;
        Context ctx; // upload context

        std::srand(std::time(0));

        std::string tmpEmail, tmpPass, tmpAddr;
        {
            guard lock(mx);
            tmpEmail = sender.GetEmail();
            tmpPass = sender.GetPassword();
            tmpAddr = smtpAddr;
        }
        ctx.readed = 0;
        ctx.payload = EmailMsg(receiversEmail, tmpEmail.c_str(), subject, body); // get email msg

        curl = curl_easy_init();
        if (curl)
        {
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, nullptr);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, nullptr);

            curl_easy_setopt(curl, CURLOPT_USERNAME, tmpEmail.c_str());
            curl_easy_setopt(curl, CURLOPT_PASSWORD, tmpPass.c_str());

            curl_easy_setopt(curl, CURLOPT_URL, tmpAddr.c_str());
            curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);

            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

            curl_easy_setopt(curl, CURLOPT_MAIL_FROM, tmpEmail.c_str());

            for (const auto& email : receiversEmail)
            {
                recipients = curl_slist_append(recipients, email.c_str()); // add all recipients to slist
            }

            curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
            curl_easy_setopt(curl, CURLOPT_READFUNCTION, PayloadSrc);
            curl_easy_setopt(curl, CURLOPT_READDATA, &ctx);
            curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

            curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);

            res = curl_easy_perform(curl);
            if (res != CURLE_OK)
            {
                DbgMsg("error curl_easy_perform(): %s\n", curl_easy_strerror(res)); // dbg curl err if send fails
            }

            curl_slist_free_all(recipients);
            curl_easy_cleanup(curl);
        }
    }

    void Request::SendAsync(const std::vector<std::string>& receiversEmail, const std::string& subject, const std::string& body) const
    {
        std::thread t(&Request::Send, this, receiversEmail, subject, body);
        t.detach();
    }

    void NotifyUsers(const std::string& subject, const std::string& msg, std::vector<std::string> emailList)
    {
        if (emailList.empty())
        {
            if (Entity::User::SelectAllUsersEmail(emailList) != Error::SUCCESSFUL) // select all customers emails if none are given
            {
                return;
            }
        }

        auto shreq = Manager::GetSMTP();
        auto shreqInst = shreq->GetInstance();

        shreqInst->SetSender(shreq->GetData().sender); // set smtp config
        shreqInst->SetServer(shreq->GetData().smtpAddr);
        shreqInst->SendAsync(emailList, subject, msg); // send email to listed users
    }
}
