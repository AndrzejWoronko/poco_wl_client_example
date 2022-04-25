#include "WLApi.h"

using Poco::Net::HTTPSClientSession;
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

using namespace Poco::JSON;
using namespace Poco::Dynamic;

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

void WLApiContractorRec::parseContractor(const std::string &str)
{
    try
    {
        Raw = str;
        Parser parser;
        Var result;
        result = parser.parse(Raw);
        Object::Ptr object = result.extract<Object::Ptr>();
        Var res = object->get("result");
        object = res.extract<Object::Ptr>();
        Var subject = object->get("subject");
        object = subject.extract<Object::Ptr>();
        Var value = object->get("name");
        if (!value.isEmpty())
            Nazwa = value.convert<std::string>();
        value = object->get("nip");
        if (!value.isEmpty())
            NIP = value.convert<std::string>();
        value = object->get("regon");
        if (!value.isEmpty())
            Regon = value.convert<std::string>();
        value = object->get("pesel");
        if (!value.isEmpty())
            Pesel = value.convert<std::string>();
        value = object->get("krs");
        if (!value.isEmpty())
            Krs = value.convert<std::string>();
        value = object->get("residenceAddress");
        if (!value.isEmpty())
            Adres = value.convert<std::string>();
        else
            {
                value = object->get("workingAddress");
                if (!value.isEmpty())
                    Adres = value.convert<std::string>();
            }
        Poco::StringTokenizer ulica_miesjcowosc(Adres, ",");
        std::string ulica = Poco::trim(ulica_miesjcowosc[0]);
        std::string miejscowosc = Poco::trim(ulica_miesjcowosc[1]);

        Ulica = Poco::trim(ulica.substr(0, ulica.find(' ')));
        Budynek = Poco::trim(ulica.substr(ulica.find(' ')));

        KodPocztowy = Poco::trim(miejscowosc.substr(0, miejscowosc.find(' ')));
        Miejscowosc =  Poco::trim(miejscowosc.substr(miejscowosc.find(' ')));

        value = object->get("statusVat");
        if (!value.isEmpty())
            StatusVat = value.convert<std::string>();

        Poco::JSON::Array::Ptr arr = object->getArray("accountNumbers");
        if (!arr.isNull())
            {
                value = arr->get(0);
                if (!value.isEmpty())
                    KontoBankowe = value.convert<std::string>();
            }
        Error = false;
    }
    catch(Poco::JSON::JSONException jsone)
    {
        Error = true;
        clear();
        ErrorInfo = jsone.message();
        std::cout << jsone.message() << std::endl;
    }
}

WLApi::WLApi()
{

}

bool WLApi::doRequest(Poco::Net::HTTPClientSession& session, Poco::Net::HTTPRequest& request, Poco::Net::HTTPResponse& response)
{
    session.setKeepAlive(true);
    request.setKeepAlive(true);
    request.setContentType("application/json");

    session.sendRequest(request);
    std::istream& rs = session.receiveResponse(response);
    std::cout << response.getStatus() << " " << response.getReason() << std::endl;
    if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
    {
        StreamCopier::copyToString(rs, m_raw_response);
        return true;
    }
    else
    {
        Poco::NullOutputStream null;
        StreamCopier::copyToString(rs, m_raw_response);
        return false;
    }
}

void WLApi::getContractorByNIP(const std::string &nip)
{
    std::stringstream path;
    LocalDateTime now;
    std::string date_str = DateTimeFormatter::format(now, "%Y-%m-%d");

    path << WL_API_GET << WL_NIP_SEARCH << nip << "?date=" << date_str;

    std::cout << "API full adress: " << path.str() << std::endl;

    Poco::URI uri(path.str());
    uri.setPort(80);
    std::string www_path(uri.getPathAndQuery());
    if (www_path.empty()) www_path = "/";

    std::cout << "API adress: " << www_path <<  std::endl;

    HTTPSClientSession session(uri.getHost());

    HTTPRequest request(HTTPRequest::HTTP_GET, www_path, HTTPMessage::HTTP_1_1);
    request.setURI(www_path);

    HTTPResponse response;

    if (this->doRequest(session, request, response))
    {
        std::cout << "Contractor with NIP: " << nip  << " found !!!" << std::endl;

        std::cout << "Raw JSON :" << m_raw_response << std::endl;

        m_contractor_rec.parseContractor(m_raw_response);

        std::cout << m_contractor_rec.toString() << std::endl;
    }
    else
    {
        std::cout << "Contractor with NIP: " << nip  << " not found !!!" << std::endl;
    }
}
