#include "vtspch.h"
#include "sqs_handler.h"

extern ConfigurationManager g_config;

void SQS::send_message_sqs(const std::string& queue_url, const std::string& msg_body) {

    Aws::SQS::SQSClient sqs;

    Aws::SQS::Model::SendMessageRequest sm_req;
    sm_req.SetQueueUrl(queue_url.c_str());
    sm_req.SetMessageBody(msg_body.c_str());

    auto sm_out = sqs.SendMessage(sm_req);
    if (sm_out.IsSuccess()) {
        std::cout << "Successfully sent message to " << queue_url <<
            std::endl;
    } else {
        std::cout << "Error sending message to " << queue_url << ": " <<
            sm_out.GetError().GetMessage() << std::endl;
    }
}
