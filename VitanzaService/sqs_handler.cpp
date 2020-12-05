#include "vtspch.h"
#include "sqs_handler.h"

extern ConfigurationManager g_config;

void SQS::send_message_sqs(const std::string& queue_url, const std::string& msg_body) {
	Aws::Auth::AWSCredentials credentials;
	Aws::Client::ClientConfiguration client_config;
	AWSUtil::client_config(credentials, client_config);

	Aws::SQS::SQSClient sqs(credentials, client_config);

	Aws::SQS::Model::SendMessageRequest sm_req;
	sm_req.SetQueueUrl(queue_url.c_str());
	sm_req.SetMessageBody(msg_body.c_str());

	auto sm_out = sqs.SendMessage(sm_req);
	if (!sm_out.IsSuccess()) {
		std::cout << "Error sending message to " << queue_url << ": " <<
			sm_out.GetError().GetMessage() << std::endl;
	}
}

std::vector<SQSMessage> SQS::receive_messages_sqs(const std::string& queue_url, const bool& delete_messages) {
	Aws::Auth::AWSCredentials credentials;
	Aws::Client::ClientConfiguration client_config;
	AWSUtil::client_config(credentials, client_config);
	
	client_config.requestTimeoutMs = 30000;

	Aws::SQS::SQSClient sqs(credentials, client_config);

	Aws::SQS::Model::ReceiveMessageRequest rm_req;
	rm_req.SetQueueUrl(queue_url.c_str());
	rm_req.SetMaxNumberOfMessages(1);


	std::vector<SQSMessage> received_messages;

	auto rm_out = sqs.ReceiveMessage(rm_req);
	if (!rm_out.IsSuccess())
	{
		std::cout << "Error receiving message from queue " << queue_url << ": "
			<< rm_out.GetError().GetMessage() << std::endl;
		return received_messages;
	}

	const auto& messages = rm_out.GetResult().GetMessages();
	if (messages.size() == 0)
	{
		std::cout << "No messages received from queue " << queue_url <<
			std::endl;
		return received_messages;
	}

	
	for (const auto& message : messages) {
		SQSMessage msg;
		msg.message_body = message.GetBody();
		msg.message_handler = message.GetReceiptHandle();
		msg.message_id = message.GetMessageId();
		received_messages.push_back(msg);
	}


	if (delete_messages) {
		for (const auto& message : received_messages) {
			Aws::SQS::Model::DeleteMessageRequest dm_req;
			dm_req.SetQueueUrl(queue_url.c_str());
			dm_req.SetReceiptHandle(message.message_handler.c_str());

			auto dm_out = sqs.DeleteMessage(dm_req);
			if (!dm_out.IsSuccess()) {
				std::cout << "Error deleting message " << message.message_id <<
					" from queue " << queue_url << ": " <<
					dm_out.GetError().GetMessage() << std::endl;
			}
		}
	}
	return received_messages;
}
