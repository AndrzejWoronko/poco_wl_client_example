#ifndef WLAPI_H
#define WLAPI_H

#include <iostream>
#include <string>
#include <sstream>

#include "Poco/LocalDateTime.h"
#include "Poco/DateTime.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeParser.h"
#include "Poco/String.h"
#include "Poco/StringTokenizer.h"

#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPCredentials.h"
#include "Poco/StreamCopier.h"
#include "Poco/NullStream.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"

#include "Poco/JSON/JSON.h"
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/ParseHandler.h"
#include "Poco/JSON/JSONException.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/Array.h"

#define ADD_FIELD(type, field, getField, setField) \
public: \
    type getField(void){return field;} \
    void setField(const type &value) {field = value;} \
    private: \
    type field;

#define WL_API_GET  "https://wl-api.mf.gov.pl"
#define WL_NIP_SEARCH "/api/search/nip/"

class  WLApiContractorRec
{
    ADD_FIELD(bool, Error, getError, setError)
    ADD_FIELD(std::string, ErrorInfo, getErrorInfo, setErrorInfo)
    ADD_FIELD(std::string, Nazwa, getNazwa, setNazwa)
    ADD_FIELD(std::string, NIP, getNip, setNip)
    ADD_FIELD(std::string, Regon, getRegon, setRegon)
    ADD_FIELD(std::string, Pesel, getPesel, setPesel)
    ADD_FIELD(std::string, Krs, getKrs, setKrs)
    ADD_FIELD(std::string, Adres, getAdres, setAdres)
    ADD_FIELD(std::string, Miejscowosc, getMiejscowosc, setMiejscowosc)
    ADD_FIELD(std::string, KodPocztowy, getKodPocztowy, setKodPocztowy)
    ADD_FIELD(std::string, Ulica, getUlica, setUlica)
    ADD_FIELD(std::string, Budynek, getBudynek, setBudynek)
    ADD_FIELD(std::string, Lokal, getLokal, setLokal)
    ADD_FIELD(std::string, StatusVat, getStatusVat, setStatusVat)
    ADD_FIELD(std::string, KontoBankowe, getKontoBankowe, setKontoBankowe)
    ADD_FIELD(std::string, RequestID, getRequestID, setRequestID)

    ADD_FIELD(std::string, Raw, getRaw, setRaw)

public:
    WLApiContractorRec();

    void clear();
    std::string toString();

    void parseContractor(const std::string &str);
};


class IGovIntegrationApi
{
public:
    virtual ~IGovIntegrationApi() = default;
    virtual void getContractorByNIP(const std::string &nip) = 0;
    virtual std::string getContractorName() = 0;
};

class WLApi : public IGovIntegrationApi
{
    WLApiContractorRec m_contractor_rec;
    std::string m_raw_response;

    bool doRequest(Poco::Net::HTTPClientSession& session, Poco::Net::HTTPRequest& request, Poco::Net::HTTPResponse& response);    

public:
    WLApi() = default;

    virtual void getContractorByNIP(const std::string &nip) override final;
    virtual std::string getContractorName() override final;

    WLApiContractorRec &getContractorRec(void) {return m_contractor_rec;}
};

#endif // WLAPI_H
