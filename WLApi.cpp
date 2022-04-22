#include "WLApi.h"

using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPMessage;
using Poco::StreamCopier;
using Poco::Path;
using Poco::URI;
using Poco::Exception;
using Poco::LocalDateTime;
using Poco::DateTime;
using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::DateTimeParser;

WLApiContractorRec::WLApiContractorRec()
{
    clear();
}

void WLApiContractorRec::clear()
{
    Error = false;
    ErrorInfo.clear();
    Nazwa.clear();
    NIP.clear();
    Regon.clear();
    Pesel.clear();
    Krs.clear();
    Adres.clear();
    Miejscowosc.clear();
    KodPocztowy.clear();
    Ulica.clear();
    Budynek.clear();
    Lokal.clear();
    StatusVat.clear();
    KontoBankowe.clear();
    RequestID.clear();
    Raw.clear();
}

std::string  WLApiContractorRec::toString()
{
    std::string  ret;
    ret += std::string ("Nazwa: ") + Nazwa + "\n";
    ret += std::string ("NIP: ") + NIP + "\n";
    ret += std::string ("Regon: ") + Regon + "\n";
    ret += std::string ("StatusVat: ") + StatusVat + "\n";
    ret += std::string ("KontoBankowe: ") + KontoBankowe + "\n";
    ret += std::string ("Adres: ") + Adres + "\n";
    ret += std::string ("Miejscowosc: ") + Miejscowosc + "\n";
    ret += std::string ("KodPocztowy: ") + KodPocztowy + "\n";
    ret += std::string ("Ulica: ") + Ulica + "\n";
    ret += std::string ("Budynek: ") + Budynek + "\n";
    ret += std::string ("Lokal: ") + Lokal + "\n";
    return ret;
}

WLApi::WLApi()
{

}

bool WLApi::doRequest(Poco::Net::HTTPClientSession& session, Poco::Net::HTTPRequest& request, Poco::Net::HTTPResponse& response)
{
    session.sendRequest(request);
    std::istream& rs = session.receiveResponse(response);
    std::cout << response.getStatus() << " " << response.getReason() << std::endl;
    if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
    {
        StreamCopier::copyStream(rs, std::cout);
        return true;
    }
    else
    {
        Poco::NullOutputStream null;
        StreamCopier::copyStream(rs, null);
        return false;
    }
}

void WLApi::getContractorByNIP(const std::string &nip)
{
    std::stringstream path;
    LocalDateTime now;
    std::string date_str = DateTimeFormatter::format(now, "%Y-%m-%d");

    path << WL_API_GET << WL_NIP_SEARCH << nip << "?date=" << date_str;

    Poco::URI uri(path.str());
    std::string www_path(uri.getPathAndQuery());

    HTTPClientSession session(uri.getHost(), uri.getPort());
    HTTPRequest request(HTTPRequest::HTTP_GET, www_path, HTTPMessage::HTTP_1_1);
    HTTPResponse response;

    if (this->doRequest(session, request, response))
    {
        std::cout << "Contractor with NIP: " << nip  << " found !!!" << std::endl;
    }
    else
    {
        std::cout << "Contractor with NIP: " << nip  << " not found !!!" << std::endl;
    }
}
