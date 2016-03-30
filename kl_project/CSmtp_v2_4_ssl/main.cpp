#include "CSmtp.h"
#include <iostream>

//int main()
extern "C" __declspec(dllexport) int sendfile()
{
	bool bError = false;
	

	try
	{
		CSmtp mail;

#define test_gmail_tls

#if defined(test_gmail_tls)
		mail.SetSMTPServer("smtp.gmail.com",587);
		mail.SetSecurityType(USE_TLS);
#elif defined(test_gmail_ssl)
		mail.SetSMTPServer("smtp.gmail.com",465);
		mail.SetSecurityType(USE_SSL);
#elif defined(test_hotmail_TLS)
		mail.SetSMTPServer("smtp.live.com",25);
		mail.SetSecurityType(USE_TLS);
#elif defined(test_aol_tls)
		mail.SetSMTPServer("smtp.aol.com",587);
		mail.SetSecurityType(USE_TLS);
#elif defined(test_yahoo_ssl)
		mail.SetSMTPServer("plus.smtp.mail.yahoo.com",465);
		mail.SetSecurityType(USE_SSL);
#endif

		mail.SetLogin("klfall2015"); /*klfall2015@gmail.com*/
		mail.SetPassword("MstRolla@321");
  		mail.SetSenderName("Network Security");
		mail.SetSenderMail("klfall2015@gmail.com");
  		mail.SetReplyTo("");
  		mail.SetSubject("The message");
  		mail.AddRecipient("klfall2015@gmail.com");
  		mail.SetXPriority(XPRIORITY_NORMAL);
  		mail.SetXMailer("The Bat! (v3.02) Professional");
  		mail.AddMsgLine("Hello,");
		mail.AddMsgLine("");
		mail.AddMsgLine("...");
		mail.AddMsgLine("How are you today?");
		mail.AddMsgLine("");
		mail.AddMsgLine("Regards");
		mail.ModMsgLine(5,"regards");
		mail.DelMsgLine(2);
		mail.AddMsgLine("User");

  		//mail.AddAttachment("../test1.jpg");
  		//mail.AddAttachment("c:\\test2.exe");
		mail.AddAttachment("G:\\ProjectFiles\\tosend\\log.txt");
		mail.Send();
	}
	catch(ECSmtp e)
	{
		std::cout << "Error: " << e.GetErrorText().c_str() << ".\n";
		MessageBoxA(NULL, "IN Sender", e.GetErrorText().c_str(), NULL);
		bError = true;
	}
	if (!bError)
	{
		std::cout << "Mail was send successfully.\n";
		MessageBoxA(NULL, "INside send file success", NULL, NULL);
	}

	return bError;
}
