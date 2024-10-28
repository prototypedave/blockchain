//
// Generated file, do not edit! Created by opp_msgtool 6.0 from inet/applications/NewBlockchain/define.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "define_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

namespace inet {

Register_Class(VehicleRA)

VehicleRA::VehicleRA() : ::inet::FieldsChunk()
{
}

VehicleRA::VehicleRA(const VehicleRA& other) : ::inet::FieldsChunk(other)
{
    copy(other);
}

VehicleRA::~VehicleRA()
{
}

VehicleRA& VehicleRA::operator=(const VehicleRA& other)
{
    if (this == &other) return *this;
    ::inet::FieldsChunk::operator=(other);
    copy(other);
    return *this;
}

void VehicleRA::copy(const VehicleRA& other)
{
    this->VehId = other.VehId;
    this->rsuId = other.rsuId;
    this->userIp = other.userIp;
    this->requestedLifetime = other.requestedLifetime;
}

void VehicleRA::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::FieldsChunk::parsimPack(b);
    doParsimPacking(b,this->VehId);
    doParsimPacking(b,this->rsuId);
    doParsimPacking(b,this->userIp);
    doParsimPacking(b,this->requestedLifetime);
}

void VehicleRA::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::FieldsChunk::parsimUnpack(b);
    doParsimUnpacking(b,this->VehId);
    doParsimUnpacking(b,this->rsuId);
    doParsimUnpacking(b,this->userIp);
    doParsimUnpacking(b,this->requestedLifetime);
}

int VehicleRA::getVehId() const
{
    return this->VehId;
}

void VehicleRA::setVehId(int VehId)
{
    handleChange();
    this->VehId = VehId;
}

int VehicleRA::getRsuId() const
{
    return this->rsuId;
}

void VehicleRA::setRsuId(int rsuId)
{
    handleChange();
    this->rsuId = rsuId;
}

const char * VehicleRA::getUserIp() const
{
    return this->userIp.c_str();
}

void VehicleRA::setUserIp(const char * userIp)
{
    handleChange();
    this->userIp = userIp;
}

int VehicleRA::getRequestedLifetime() const
{
    return this->requestedLifetime;
}

void VehicleRA::setRequestedLifetime(int requestedLifetime)
{
    handleChange();
    this->requestedLifetime = requestedLifetime;
}

class VehicleRADescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_VehId,
        FIELD_rsuId,
        FIELD_userIp,
        FIELD_requestedLifetime,
    };
  public:
    VehicleRADescriptor();
    virtual ~VehicleRADescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(VehicleRADescriptor)

VehicleRADescriptor::VehicleRADescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::VehicleRA)), "inet::FieldsChunk")
{
    propertyNames = nullptr;
}

VehicleRADescriptor::~VehicleRADescriptor()
{
    delete[] propertyNames;
}

bool VehicleRADescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<VehicleRA *>(obj)!=nullptr;
}

const char **VehicleRADescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *VehicleRADescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int VehicleRADescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 4+base->getFieldCount() : 4;
}

unsigned int VehicleRADescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_VehId
        FD_ISEDITABLE,    // FIELD_rsuId
        FD_ISEDITABLE,    // FIELD_userIp
        FD_ISEDITABLE,    // FIELD_requestedLifetime
    };
    return (field >= 0 && field < 4) ? fieldTypeFlags[field] : 0;
}

const char *VehicleRADescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "VehId",
        "rsuId",
        "userIp",
        "requestedLifetime",
    };
    return (field >= 0 && field < 4) ? fieldNames[field] : nullptr;
}

int VehicleRADescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "VehId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "rsuId") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "userIp") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "requestedLifetime") == 0) return baseIndex + 3;
    return base ? base->findField(fieldName) : -1;
}

const char *VehicleRADescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_VehId
        "int",    // FIELD_rsuId
        "string",    // FIELD_userIp
        "int",    // FIELD_requestedLifetime
    };
    return (field >= 0 && field < 4) ? fieldTypeStrings[field] : nullptr;
}

const char **VehicleRADescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *VehicleRADescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int VehicleRADescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    VehicleRA *pp = omnetpp::fromAnyPtr<VehicleRA>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void VehicleRADescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    VehicleRA *pp = omnetpp::fromAnyPtr<VehicleRA>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'VehicleRA'", field);
    }
}

const char *VehicleRADescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    VehicleRA *pp = omnetpp::fromAnyPtr<VehicleRA>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string VehicleRADescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    VehicleRA *pp = omnetpp::fromAnyPtr<VehicleRA>(object); (void)pp;
    switch (field) {
        case FIELD_VehId: return long2string(pp->getVehId());
        case FIELD_rsuId: return long2string(pp->getRsuId());
        case FIELD_userIp: return oppstring2string(pp->getUserIp());
        case FIELD_requestedLifetime: return long2string(pp->getRequestedLifetime());
        default: return "";
    }
}

void VehicleRADescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    VehicleRA *pp = omnetpp::fromAnyPtr<VehicleRA>(object); (void)pp;
    switch (field) {
        case FIELD_VehId: pp->setVehId(string2long(value)); break;
        case FIELD_rsuId: pp->setRsuId(string2long(value)); break;
        case FIELD_userIp: pp->setUserIp((value)); break;
        case FIELD_requestedLifetime: pp->setRequestedLifetime(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'VehicleRA'", field);
    }
}

omnetpp::cValue VehicleRADescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    VehicleRA *pp = omnetpp::fromAnyPtr<VehicleRA>(object); (void)pp;
    switch (field) {
        case FIELD_VehId: return pp->getVehId();
        case FIELD_rsuId: return pp->getRsuId();
        case FIELD_userIp: return pp->getUserIp();
        case FIELD_requestedLifetime: return pp->getRequestedLifetime();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'VehicleRA' as cValue -- field index out of range?", field);
    }
}

void VehicleRADescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    VehicleRA *pp = omnetpp::fromAnyPtr<VehicleRA>(object); (void)pp;
    switch (field) {
        case FIELD_VehId: pp->setVehId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_rsuId: pp->setRsuId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_userIp: pp->setUserIp(value.stringValue()); break;
        case FIELD_requestedLifetime: pp->setRequestedLifetime(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'VehicleRA'", field);
    }
}

const char *VehicleRADescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr VehicleRADescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    VehicleRA *pp = omnetpp::fromAnyPtr<VehicleRA>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void VehicleRADescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    VehicleRA *pp = omnetpp::fromAnyPtr<VehicleRA>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'VehicleRA'", field);
    }
}

Register_Class(ASAT)

ASAT::ASAT() : ::inet::FieldsChunk()
{
}

ASAT::ASAT(const ASAT& other) : ::inet::FieldsChunk(other)
{
    copy(other);
}

ASAT::~ASAT()
{
}

ASAT& ASAT::operator=(const ASAT& other)
{
    if (this == &other) return *this;
    ::inet::FieldsChunk::operator=(other);
    copy(other);
    return *this;
}

void ASAT::copy(const ASAT& other)
{
    this->AT = other.AT;
    this->secret = other.secret;
    this->vehId = other.vehId;
}

void ASAT::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::FieldsChunk::parsimPack(b);
    doParsimPacking(b,this->AT);
    doParsimPacking(b,this->secret);
    doParsimPacking(b,this->vehId);
}

void ASAT::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::FieldsChunk::parsimUnpack(b);
    doParsimUnpacking(b,this->AT);
    doParsimUnpacking(b,this->secret);
    doParsimUnpacking(b,this->vehId);
}

const char * ASAT::getAT() const
{
    return this->AT.c_str();
}

void ASAT::setAT(const char * AT)
{
    handleChange();
    this->AT = AT;
}

const char * ASAT::getSecret() const
{
    return this->secret.c_str();
}

void ASAT::setSecret(const char * secret)
{
    handleChange();
    this->secret = secret;
}

int ASAT::getVehId() const
{
    return this->vehId;
}

void ASAT::setVehId(int vehId)
{
    handleChange();
    this->vehId = vehId;
}

class ASATDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_AT,
        FIELD_secret,
        FIELD_vehId,
    };
  public:
    ASATDescriptor();
    virtual ~ASATDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(ASATDescriptor)

ASATDescriptor::ASATDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::ASAT)), "inet::FieldsChunk")
{
    propertyNames = nullptr;
}

ASATDescriptor::~ASATDescriptor()
{
    delete[] propertyNames;
}

bool ASATDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<ASAT *>(obj)!=nullptr;
}

const char **ASATDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *ASATDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int ASATDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 3+base->getFieldCount() : 3;
}

unsigned int ASATDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_AT
        FD_ISEDITABLE,    // FIELD_secret
        FD_ISEDITABLE,    // FIELD_vehId
    };
    return (field >= 0 && field < 3) ? fieldTypeFlags[field] : 0;
}

const char *ASATDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "AT",
        "secret",
        "vehId",
    };
    return (field >= 0 && field < 3) ? fieldNames[field] : nullptr;
}

int ASATDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "AT") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "secret") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "vehId") == 0) return baseIndex + 2;
    return base ? base->findField(fieldName) : -1;
}

const char *ASATDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "string",    // FIELD_AT
        "string",    // FIELD_secret
        "int",    // FIELD_vehId
    };
    return (field >= 0 && field < 3) ? fieldTypeStrings[field] : nullptr;
}

const char **ASATDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *ASATDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int ASATDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    ASAT *pp = omnetpp::fromAnyPtr<ASAT>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void ASATDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    ASAT *pp = omnetpp::fromAnyPtr<ASAT>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'ASAT'", field);
    }
}

const char *ASATDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    ASAT *pp = omnetpp::fromAnyPtr<ASAT>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string ASATDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    ASAT *pp = omnetpp::fromAnyPtr<ASAT>(object); (void)pp;
    switch (field) {
        case FIELD_AT: return oppstring2string(pp->getAT());
        case FIELD_secret: return oppstring2string(pp->getSecret());
        case FIELD_vehId: return long2string(pp->getVehId());
        default: return "";
    }
}

void ASATDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    ASAT *pp = omnetpp::fromAnyPtr<ASAT>(object); (void)pp;
    switch (field) {
        case FIELD_AT: pp->setAT((value)); break;
        case FIELD_secret: pp->setSecret((value)); break;
        case FIELD_vehId: pp->setVehId(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ASAT'", field);
    }
}

omnetpp::cValue ASATDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    ASAT *pp = omnetpp::fromAnyPtr<ASAT>(object); (void)pp;
    switch (field) {
        case FIELD_AT: return pp->getAT();
        case FIELD_secret: return pp->getSecret();
        case FIELD_vehId: return pp->getVehId();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'ASAT' as cValue -- field index out of range?", field);
    }
}

void ASATDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    ASAT *pp = omnetpp::fromAnyPtr<ASAT>(object); (void)pp;
    switch (field) {
        case FIELD_AT: pp->setAT(value.stringValue()); break;
        case FIELD_secret: pp->setSecret(value.stringValue()); break;
        case FIELD_vehId: pp->setVehId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ASAT'", field);
    }
}

const char *ASATDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr ASATDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    ASAT *pp = omnetpp::fromAnyPtr<ASAT>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void ASATDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    ASAT *pp = omnetpp::fromAnyPtr<ASAT>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ASAT'", field);
    }
}

Register_Class(TGT)

TGT::TGT() : ::inet::FieldsChunk()
{
}

TGT::TGT(const TGT& other) : ::inet::FieldsChunk(other)
{
    copy(other);
}

TGT::~TGT()
{
}

TGT& TGT::operator=(const TGT& other)
{
    if (this == &other) return *this;
    ::inet::FieldsChunk::operator=(other);
    copy(other);
    return *this;
}

void TGT::copy(const TGT& other)
{
    this->tGT = other.tGT;
    this->secret = other.secret;
}

void TGT::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::FieldsChunk::parsimPack(b);
    doParsimPacking(b,this->tGT);
    doParsimPacking(b,this->secret);
}

void TGT::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::FieldsChunk::parsimUnpack(b);
    doParsimUnpacking(b,this->tGT);
    doParsimUnpacking(b,this->secret);
}

const char * TGT::getTGT() const
{
    return this->tGT.c_str();
}

void TGT::setTGT(const char * tGT)
{
    handleChange();
    this->tGT = tGT;
}

const char * TGT::getSecret() const
{
    return this->secret.c_str();
}

void TGT::setSecret(const char * secret)
{
    handleChange();
    this->secret = secret;
}

class TGTDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_tGT,
        FIELD_secret,
    };
  public:
    TGTDescriptor();
    virtual ~TGTDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(TGTDescriptor)

TGTDescriptor::TGTDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::TGT)), "inet::FieldsChunk")
{
    propertyNames = nullptr;
}

TGTDescriptor::~TGTDescriptor()
{
    delete[] propertyNames;
}

bool TGTDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<TGT *>(obj)!=nullptr;
}

const char **TGTDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *TGTDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int TGTDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 2+base->getFieldCount() : 2;
}

unsigned int TGTDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_tGT
        FD_ISEDITABLE,    // FIELD_secret
    };
    return (field >= 0 && field < 2) ? fieldTypeFlags[field] : 0;
}

const char *TGTDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "tGT",
        "secret",
    };
    return (field >= 0 && field < 2) ? fieldNames[field] : nullptr;
}

int TGTDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "tGT") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "secret") == 0) return baseIndex + 1;
    return base ? base->findField(fieldName) : -1;
}

const char *TGTDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "string",    // FIELD_tGT
        "string",    // FIELD_secret
    };
    return (field >= 0 && field < 2) ? fieldTypeStrings[field] : nullptr;
}

const char **TGTDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *TGTDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int TGTDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    TGT *pp = omnetpp::fromAnyPtr<TGT>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void TGTDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    TGT *pp = omnetpp::fromAnyPtr<TGT>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'TGT'", field);
    }
}

const char *TGTDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    TGT *pp = omnetpp::fromAnyPtr<TGT>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string TGTDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    TGT *pp = omnetpp::fromAnyPtr<TGT>(object); (void)pp;
    switch (field) {
        case FIELD_tGT: return oppstring2string(pp->getTGT());
        case FIELD_secret: return oppstring2string(pp->getSecret());
        default: return "";
    }
}

void TGTDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    TGT *pp = omnetpp::fromAnyPtr<TGT>(object); (void)pp;
    switch (field) {
        case FIELD_tGT: pp->setTGT((value)); break;
        case FIELD_secret: pp->setSecret((value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'TGT'", field);
    }
}

omnetpp::cValue TGTDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    TGT *pp = omnetpp::fromAnyPtr<TGT>(object); (void)pp;
    switch (field) {
        case FIELD_tGT: return pp->getTGT();
        case FIELD_secret: return pp->getSecret();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'TGT' as cValue -- field index out of range?", field);
    }
}

void TGTDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    TGT *pp = omnetpp::fromAnyPtr<TGT>(object); (void)pp;
    switch (field) {
        case FIELD_tGT: pp->setTGT(value.stringValue()); break;
        case FIELD_secret: pp->setSecret(value.stringValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'TGT'", field);
    }
}

const char *TGTDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr TGTDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    TGT *pp = omnetpp::fromAnyPtr<TGT>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void TGTDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    TGT *pp = omnetpp::fromAnyPtr<TGT>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'TGT'", field);
    }
}

Register_Class(VehicleAT)

VehicleAT::VehicleAT() : ::inet::FieldsChunk()
{
}

VehicleAT::VehicleAT(const VehicleAT& other) : ::inet::FieldsChunk(other)
{
    copy(other);
}

VehicleAT::~VehicleAT()
{
}

VehicleAT& VehicleAT::operator=(const VehicleAT& other)
{
    if (this == &other) return *this;
    ::inet::FieldsChunk::operator=(other);
    copy(other);
    return *this;
}

void VehicleAT::copy(const VehicleAT& other)
{
    this->serviceName = other.serviceName;
    this->LTReqTGT = other.LTReqTGT;
}

void VehicleAT::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::FieldsChunk::parsimPack(b);
    doParsimPacking(b,this->serviceName);
    doParsimPacking(b,this->LTReqTGT);
}

void VehicleAT::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::FieldsChunk::parsimUnpack(b);
    doParsimUnpacking(b,this->serviceName);
    doParsimUnpacking(b,this->LTReqTGT);
}

int VehicleAT::getServiceName() const
{
    return this->serviceName;
}

void VehicleAT::setServiceName(int serviceName)
{
    handleChange();
    this->serviceName = serviceName;
}

int VehicleAT::getLTReqTGT() const
{
    return this->LTReqTGT;
}

void VehicleAT::setLTReqTGT(int LTReqTGT)
{
    handleChange();
    this->LTReqTGT = LTReqTGT;
}

class VehicleATDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_serviceName,
        FIELD_LTReqTGT,
    };
  public:
    VehicleATDescriptor();
    virtual ~VehicleATDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(VehicleATDescriptor)

VehicleATDescriptor::VehicleATDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::VehicleAT)), "inet::FieldsChunk")
{
    propertyNames = nullptr;
}

VehicleATDescriptor::~VehicleATDescriptor()
{
    delete[] propertyNames;
}

bool VehicleATDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<VehicleAT *>(obj)!=nullptr;
}

const char **VehicleATDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *VehicleATDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int VehicleATDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 2+base->getFieldCount() : 2;
}

unsigned int VehicleATDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_serviceName
        FD_ISEDITABLE,    // FIELD_LTReqTGT
    };
    return (field >= 0 && field < 2) ? fieldTypeFlags[field] : 0;
}

const char *VehicleATDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "serviceName",
        "LTReqTGT",
    };
    return (field >= 0 && field < 2) ? fieldNames[field] : nullptr;
}

int VehicleATDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "serviceName") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "LTReqTGT") == 0) return baseIndex + 1;
    return base ? base->findField(fieldName) : -1;
}

const char *VehicleATDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_serviceName
        "int",    // FIELD_LTReqTGT
    };
    return (field >= 0 && field < 2) ? fieldTypeStrings[field] : nullptr;
}

const char **VehicleATDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *VehicleATDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int VehicleATDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    VehicleAT *pp = omnetpp::fromAnyPtr<VehicleAT>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void VehicleATDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    VehicleAT *pp = omnetpp::fromAnyPtr<VehicleAT>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'VehicleAT'", field);
    }
}

const char *VehicleATDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    VehicleAT *pp = omnetpp::fromAnyPtr<VehicleAT>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string VehicleATDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    VehicleAT *pp = omnetpp::fromAnyPtr<VehicleAT>(object); (void)pp;
    switch (field) {
        case FIELD_serviceName: return long2string(pp->getServiceName());
        case FIELD_LTReqTGT: return long2string(pp->getLTReqTGT());
        default: return "";
    }
}

void VehicleATDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    VehicleAT *pp = omnetpp::fromAnyPtr<VehicleAT>(object); (void)pp;
    switch (field) {
        case FIELD_serviceName: pp->setServiceName(string2long(value)); break;
        case FIELD_LTReqTGT: pp->setLTReqTGT(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'VehicleAT'", field);
    }
}

omnetpp::cValue VehicleATDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    VehicleAT *pp = omnetpp::fromAnyPtr<VehicleAT>(object); (void)pp;
    switch (field) {
        case FIELD_serviceName: return pp->getServiceName();
        case FIELD_LTReqTGT: return pp->getLTReqTGT();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'VehicleAT' as cValue -- field index out of range?", field);
    }
}

void VehicleATDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    VehicleAT *pp = omnetpp::fromAnyPtr<VehicleAT>(object); (void)pp;
    switch (field) {
        case FIELD_serviceName: pp->setServiceName(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_LTReqTGT: pp->setLTReqTGT(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'VehicleAT'", field);
    }
}

const char *VehicleATDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr VehicleATDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    VehicleAT *pp = omnetpp::fromAnyPtr<VehicleAT>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void VehicleATDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    VehicleAT *pp = omnetpp::fromAnyPtr<VehicleAT>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'VehicleAT'", field);
    }
}

Register_Class(VehicleAU)

VehicleAU::VehicleAU() : ::inet::FieldsChunk()
{
}

VehicleAU::VehicleAU(const VehicleAU& other) : ::inet::FieldsChunk(other)
{
    copy(other);
}

VehicleAU::~VehicleAU()
{
}

VehicleAU& VehicleAU::operator=(const VehicleAU& other)
{
    if (this == &other) return *this;
    ::inet::FieldsChunk::operator=(other);
    copy(other);
    return *this;
}

void VehicleAU::copy(const VehicleAU& other)
{
    this->AU = other.AU;
    this->secret = other.secret;
}

void VehicleAU::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::FieldsChunk::parsimPack(b);
    doParsimPacking(b,this->AU);
    doParsimPacking(b,this->secret);
}

void VehicleAU::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::FieldsChunk::parsimUnpack(b);
    doParsimUnpacking(b,this->AU);
    doParsimUnpacking(b,this->secret);
}

const char * VehicleAU::getAU() const
{
    return this->AU.c_str();
}

void VehicleAU::setAU(const char * AU)
{
    handleChange();
    this->AU = AU;
}

const char * VehicleAU::getSecret() const
{
    return this->secret.c_str();
}

void VehicleAU::setSecret(const char * secret)
{
    handleChange();
    this->secret = secret;
}

class VehicleAUDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_AU,
        FIELD_secret,
    };
  public:
    VehicleAUDescriptor();
    virtual ~VehicleAUDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(VehicleAUDescriptor)

VehicleAUDescriptor::VehicleAUDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::VehicleAU)), "inet::FieldsChunk")
{
    propertyNames = nullptr;
}

VehicleAUDescriptor::~VehicleAUDescriptor()
{
    delete[] propertyNames;
}

bool VehicleAUDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<VehicleAU *>(obj)!=nullptr;
}

const char **VehicleAUDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *VehicleAUDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int VehicleAUDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 2+base->getFieldCount() : 2;
}

unsigned int VehicleAUDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_AU
        FD_ISEDITABLE,    // FIELD_secret
    };
    return (field >= 0 && field < 2) ? fieldTypeFlags[field] : 0;
}

const char *VehicleAUDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "AU",
        "secret",
    };
    return (field >= 0 && field < 2) ? fieldNames[field] : nullptr;
}

int VehicleAUDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "AU") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "secret") == 0) return baseIndex + 1;
    return base ? base->findField(fieldName) : -1;
}

const char *VehicleAUDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "string",    // FIELD_AU
        "string",    // FIELD_secret
    };
    return (field >= 0 && field < 2) ? fieldTypeStrings[field] : nullptr;
}

const char **VehicleAUDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *VehicleAUDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int VehicleAUDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    VehicleAU *pp = omnetpp::fromAnyPtr<VehicleAU>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void VehicleAUDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    VehicleAU *pp = omnetpp::fromAnyPtr<VehicleAU>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'VehicleAU'", field);
    }
}

const char *VehicleAUDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    VehicleAU *pp = omnetpp::fromAnyPtr<VehicleAU>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string VehicleAUDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    VehicleAU *pp = omnetpp::fromAnyPtr<VehicleAU>(object); (void)pp;
    switch (field) {
        case FIELD_AU: return oppstring2string(pp->getAU());
        case FIELD_secret: return oppstring2string(pp->getSecret());
        default: return "";
    }
}

void VehicleAUDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    VehicleAU *pp = omnetpp::fromAnyPtr<VehicleAU>(object); (void)pp;
    switch (field) {
        case FIELD_AU: pp->setAU((value)); break;
        case FIELD_secret: pp->setSecret((value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'VehicleAU'", field);
    }
}

omnetpp::cValue VehicleAUDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    VehicleAU *pp = omnetpp::fromAnyPtr<VehicleAU>(object); (void)pp;
    switch (field) {
        case FIELD_AU: return pp->getAU();
        case FIELD_secret: return pp->getSecret();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'VehicleAU' as cValue -- field index out of range?", field);
    }
}

void VehicleAUDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    VehicleAU *pp = omnetpp::fromAnyPtr<VehicleAU>(object); (void)pp;
    switch (field) {
        case FIELD_AU: pp->setAU(value.stringValue()); break;
        case FIELD_secret: pp->setSecret(value.stringValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'VehicleAU'", field);
    }
}

const char *VehicleAUDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr VehicleAUDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    VehicleAU *pp = omnetpp::fromAnyPtr<VehicleAU>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void VehicleAUDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    VehicleAU *pp = omnetpp::fromAnyPtr<VehicleAU>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'VehicleAU'", field);
    }
}

Register_Class(CHAIN)

CHAIN::CHAIN() : ::inet::FieldsChunk()
{
}

CHAIN::CHAIN(const CHAIN& other) : ::inet::FieldsChunk(other)
{
    copy(other);
}

CHAIN::~CHAIN()
{
}

CHAIN& CHAIN::operator=(const CHAIN& other)
{
    if (this == &other) return *this;
    ::inet::FieldsChunk::operator=(other);
    copy(other);
    return *this;
}

void CHAIN::copy(const CHAIN& other)
{
    this->AU = other.AU;
    this->vehId = other.vehId;
    this->destId = other.destId;
}

void CHAIN::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::FieldsChunk::parsimPack(b);
    doParsimPacking(b,this->AU);
    doParsimPacking(b,this->vehId);
    doParsimPacking(b,this->destId);
}

void CHAIN::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::FieldsChunk::parsimUnpack(b);
    doParsimUnpacking(b,this->AU);
    doParsimUnpacking(b,this->vehId);
    doParsimUnpacking(b,this->destId);
}

const char * CHAIN::getAU() const
{
    return this->AU.c_str();
}

void CHAIN::setAU(const char * AU)
{
    handleChange();
    this->AU = AU;
}

const char * CHAIN::getVehId() const
{
    return this->vehId.c_str();
}

void CHAIN::setVehId(const char * vehId)
{
    handleChange();
    this->vehId = vehId;
}

const char * CHAIN::getDestId() const
{
    return this->destId.c_str();
}

void CHAIN::setDestId(const char * destId)
{
    handleChange();
    this->destId = destId;
}

class CHAINDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_AU,
        FIELD_vehId,
        FIELD_destId,
    };
  public:
    CHAINDescriptor();
    virtual ~CHAINDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(CHAINDescriptor)

CHAINDescriptor::CHAINDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::CHAIN)), "inet::FieldsChunk")
{
    propertyNames = nullptr;
}

CHAINDescriptor::~CHAINDescriptor()
{
    delete[] propertyNames;
}

bool CHAINDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<CHAIN *>(obj)!=nullptr;
}

const char **CHAINDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *CHAINDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int CHAINDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 3+base->getFieldCount() : 3;
}

unsigned int CHAINDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_AU
        FD_ISEDITABLE,    // FIELD_vehId
        FD_ISEDITABLE,    // FIELD_destId
    };
    return (field >= 0 && field < 3) ? fieldTypeFlags[field] : 0;
}

const char *CHAINDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "AU",
        "vehId",
        "destId",
    };
    return (field >= 0 && field < 3) ? fieldNames[field] : nullptr;
}

int CHAINDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "AU") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "vehId") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "destId") == 0) return baseIndex + 2;
    return base ? base->findField(fieldName) : -1;
}

const char *CHAINDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "string",    // FIELD_AU
        "string",    // FIELD_vehId
        "string",    // FIELD_destId
    };
    return (field >= 0 && field < 3) ? fieldTypeStrings[field] : nullptr;
}

const char **CHAINDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *CHAINDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int CHAINDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    CHAIN *pp = omnetpp::fromAnyPtr<CHAIN>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void CHAINDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    CHAIN *pp = omnetpp::fromAnyPtr<CHAIN>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'CHAIN'", field);
    }
}

const char *CHAINDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    CHAIN *pp = omnetpp::fromAnyPtr<CHAIN>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string CHAINDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    CHAIN *pp = omnetpp::fromAnyPtr<CHAIN>(object); (void)pp;
    switch (field) {
        case FIELD_AU: return oppstring2string(pp->getAU());
        case FIELD_vehId: return oppstring2string(pp->getVehId());
        case FIELD_destId: return oppstring2string(pp->getDestId());
        default: return "";
    }
}

void CHAINDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    CHAIN *pp = omnetpp::fromAnyPtr<CHAIN>(object); (void)pp;
    switch (field) {
        case FIELD_AU: pp->setAU((value)); break;
        case FIELD_vehId: pp->setVehId((value)); break;
        case FIELD_destId: pp->setDestId((value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'CHAIN'", field);
    }
}

omnetpp::cValue CHAINDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    CHAIN *pp = omnetpp::fromAnyPtr<CHAIN>(object); (void)pp;
    switch (field) {
        case FIELD_AU: return pp->getAU();
        case FIELD_vehId: return pp->getVehId();
        case FIELD_destId: return pp->getDestId();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'CHAIN' as cValue -- field index out of range?", field);
    }
}

void CHAINDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    CHAIN *pp = omnetpp::fromAnyPtr<CHAIN>(object); (void)pp;
    switch (field) {
        case FIELD_AU: pp->setAU(value.stringValue()); break;
        case FIELD_vehId: pp->setVehId(value.stringValue()); break;
        case FIELD_destId: pp->setDestId(value.stringValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'CHAIN'", field);
    }
}

const char *CHAINDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr CHAINDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    CHAIN *pp = omnetpp::fromAnyPtr<CHAIN>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void CHAINDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    CHAIN *pp = omnetpp::fromAnyPtr<CHAIN>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'CHAIN'", field);
    }
}

Register_Class(TGSAT)

TGSAT::TGSAT() : ::inet::FieldsChunk()
{
}

TGSAT::TGSAT(const TGSAT& other) : ::inet::FieldsChunk(other)
{
    copy(other);
}

TGSAT::~TGSAT()
{
}

TGSAT& TGSAT::operator=(const TGSAT& other)
{
    if (this == &other) return *this;
    ::inet::FieldsChunk::operator=(other);
    copy(other);
    return *this;
}

void TGSAT::copy(const TGSAT& other)
{
    this->AT = other.AT;
    this->secret = other.secret;
}

void TGSAT::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::FieldsChunk::parsimPack(b);
    doParsimPacking(b,this->AT);
    doParsimPacking(b,this->secret);
}

void TGSAT::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::FieldsChunk::parsimUnpack(b);
    doParsimUnpacking(b,this->AT);
    doParsimUnpacking(b,this->secret);
}

const char * TGSAT::getAT() const
{
    return this->AT.c_str();
}

void TGSAT::setAT(const char * AT)
{
    handleChange();
    this->AT = AT;
}

const char * TGSAT::getSecret() const
{
    return this->secret.c_str();
}

void TGSAT::setSecret(const char * secret)
{
    handleChange();
    this->secret = secret;
}

class TGSATDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_AT,
        FIELD_secret,
    };
  public:
    TGSATDescriptor();
    virtual ~TGSATDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(TGSATDescriptor)

TGSATDescriptor::TGSATDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::TGSAT)), "inet::FieldsChunk")
{
    propertyNames = nullptr;
}

TGSATDescriptor::~TGSATDescriptor()
{
    delete[] propertyNames;
}

bool TGSATDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<TGSAT *>(obj)!=nullptr;
}

const char **TGSATDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *TGSATDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int TGSATDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 2+base->getFieldCount() : 2;
}

unsigned int TGSATDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_AT
        FD_ISEDITABLE,    // FIELD_secret
    };
    return (field >= 0 && field < 2) ? fieldTypeFlags[field] : 0;
}

const char *TGSATDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "AT",
        "secret",
    };
    return (field >= 0 && field < 2) ? fieldNames[field] : nullptr;
}

int TGSATDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "AT") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "secret") == 0) return baseIndex + 1;
    return base ? base->findField(fieldName) : -1;
}

const char *TGSATDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "string",    // FIELD_AT
        "string",    // FIELD_secret
    };
    return (field >= 0 && field < 2) ? fieldTypeStrings[field] : nullptr;
}

const char **TGSATDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *TGSATDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int TGSATDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    TGSAT *pp = omnetpp::fromAnyPtr<TGSAT>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void TGSATDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    TGSAT *pp = omnetpp::fromAnyPtr<TGSAT>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'TGSAT'", field);
    }
}

const char *TGSATDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    TGSAT *pp = omnetpp::fromAnyPtr<TGSAT>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string TGSATDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    TGSAT *pp = omnetpp::fromAnyPtr<TGSAT>(object); (void)pp;
    switch (field) {
        case FIELD_AT: return oppstring2string(pp->getAT());
        case FIELD_secret: return oppstring2string(pp->getSecret());
        default: return "";
    }
}

void TGSATDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    TGSAT *pp = omnetpp::fromAnyPtr<TGSAT>(object); (void)pp;
    switch (field) {
        case FIELD_AT: pp->setAT((value)); break;
        case FIELD_secret: pp->setSecret((value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'TGSAT'", field);
    }
}

omnetpp::cValue TGSATDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    TGSAT *pp = omnetpp::fromAnyPtr<TGSAT>(object); (void)pp;
    switch (field) {
        case FIELD_AT: return pp->getAT();
        case FIELD_secret: return pp->getSecret();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'TGSAT' as cValue -- field index out of range?", field);
    }
}

void TGSATDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    TGSAT *pp = omnetpp::fromAnyPtr<TGSAT>(object); (void)pp;
    switch (field) {
        case FIELD_AT: pp->setAT(value.stringValue()); break;
        case FIELD_secret: pp->setSecret(value.stringValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'TGSAT'", field);
    }
}

const char *TGSATDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr TGSATDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    TGSAT *pp = omnetpp::fromAnyPtr<TGSAT>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void TGSATDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    TGSAT *pp = omnetpp::fromAnyPtr<TGSAT>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'TGSAT'", field);
    }
}

Register_Class(STPacket)

STPacket::STPacket() : ::inet::FieldsChunk()
{
}

STPacket::STPacket(const STPacket& other) : ::inet::FieldsChunk(other)
{
    copy(other);
}

STPacket::~STPacket()
{
}

STPacket& STPacket::operator=(const STPacket& other)
{
    if (this == &other) return *this;
    ::inet::FieldsChunk::operator=(other);
    copy(other);
    return *this;
}

void STPacket::copy(const STPacket& other)
{
    this->sT = other.sT;
    this->secret = other.secret;
}

void STPacket::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::FieldsChunk::parsimPack(b);
    doParsimPacking(b,this->sT);
    doParsimPacking(b,this->secret);
}

void STPacket::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::FieldsChunk::parsimUnpack(b);
    doParsimUnpacking(b,this->sT);
    doParsimUnpacking(b,this->secret);
}

const char * STPacket::getST() const
{
    return this->sT.c_str();
}

void STPacket::setST(const char * sT)
{
    handleChange();
    this->sT = sT;
}

const char * STPacket::getSecret() const
{
    return this->secret.c_str();
}

void STPacket::setSecret(const char * secret)
{
    handleChange();
    this->secret = secret;
}

class STPacketDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_sT,
        FIELD_secret,
    };
  public:
    STPacketDescriptor();
    virtual ~STPacketDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(STPacketDescriptor)

STPacketDescriptor::STPacketDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::STPacket)), "inet::FieldsChunk")
{
    propertyNames = nullptr;
}

STPacketDescriptor::~STPacketDescriptor()
{
    delete[] propertyNames;
}

bool STPacketDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<STPacket *>(obj)!=nullptr;
}

const char **STPacketDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *STPacketDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int STPacketDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 2+base->getFieldCount() : 2;
}

unsigned int STPacketDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_sT
        FD_ISEDITABLE,    // FIELD_secret
    };
    return (field >= 0 && field < 2) ? fieldTypeFlags[field] : 0;
}

const char *STPacketDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "sT",
        "secret",
    };
    return (field >= 0 && field < 2) ? fieldNames[field] : nullptr;
}

int STPacketDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "sT") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "secret") == 0) return baseIndex + 1;
    return base ? base->findField(fieldName) : -1;
}

const char *STPacketDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "string",    // FIELD_sT
        "string",    // FIELD_secret
    };
    return (field >= 0 && field < 2) ? fieldTypeStrings[field] : nullptr;
}

const char **STPacketDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *STPacketDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int STPacketDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    STPacket *pp = omnetpp::fromAnyPtr<STPacket>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void STPacketDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    STPacket *pp = omnetpp::fromAnyPtr<STPacket>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'STPacket'", field);
    }
}

const char *STPacketDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    STPacket *pp = omnetpp::fromAnyPtr<STPacket>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string STPacketDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    STPacket *pp = omnetpp::fromAnyPtr<STPacket>(object); (void)pp;
    switch (field) {
        case FIELD_sT: return oppstring2string(pp->getST());
        case FIELD_secret: return oppstring2string(pp->getSecret());
        default: return "";
    }
}

void STPacketDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    STPacket *pp = omnetpp::fromAnyPtr<STPacket>(object); (void)pp;
    switch (field) {
        case FIELD_sT: pp->setST((value)); break;
        case FIELD_secret: pp->setSecret((value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'STPacket'", field);
    }
}

omnetpp::cValue STPacketDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    STPacket *pp = omnetpp::fromAnyPtr<STPacket>(object); (void)pp;
    switch (field) {
        case FIELD_sT: return pp->getST();
        case FIELD_secret: return pp->getSecret();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'STPacket' as cValue -- field index out of range?", field);
    }
}

void STPacketDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    STPacket *pp = omnetpp::fromAnyPtr<STPacket>(object); (void)pp;
    switch (field) {
        case FIELD_sT: pp->setST(value.stringValue()); break;
        case FIELD_secret: pp->setSecret(value.stringValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'STPacket'", field);
    }
}

const char *STPacketDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr STPacketDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    STPacket *pp = omnetpp::fromAnyPtr<STPacket>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void STPacketDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    STPacket *pp = omnetpp::fromAnyPtr<STPacket>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'STPacket'", field);
    }
}

Register_Class(VehicleToServerAU)

VehicleToServerAU::VehicleToServerAU() : ::inet::FieldsChunk()
{
}

VehicleToServerAU::VehicleToServerAU(const VehicleToServerAU& other) : ::inet::FieldsChunk(other)
{
    copy(other);
}

VehicleToServerAU::~VehicleToServerAU()
{
}

VehicleToServerAU& VehicleToServerAU::operator=(const VehicleToServerAU& other)
{
    if (this == &other) return *this;
    ::inet::FieldsChunk::operator=(other);
    copy(other);
    return *this;
}

void VehicleToServerAU::copy(const VehicleToServerAU& other)
{
    this->AU = other.AU;
    this->secret = other.secret;
}

void VehicleToServerAU::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::FieldsChunk::parsimPack(b);
    doParsimPacking(b,this->AU);
    doParsimPacking(b,this->secret);
}

void VehicleToServerAU::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::FieldsChunk::parsimUnpack(b);
    doParsimUnpacking(b,this->AU);
    doParsimUnpacking(b,this->secret);
}

const char * VehicleToServerAU::getAU() const
{
    return this->AU.c_str();
}

void VehicleToServerAU::setAU(const char * AU)
{
    handleChange();
    this->AU = AU;
}

const char * VehicleToServerAU::getSecret() const
{
    return this->secret.c_str();
}

void VehicleToServerAU::setSecret(const char * secret)
{
    handleChange();
    this->secret = secret;
}

class VehicleToServerAUDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_AU,
        FIELD_secret,
    };
  public:
    VehicleToServerAUDescriptor();
    virtual ~VehicleToServerAUDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(VehicleToServerAUDescriptor)

VehicleToServerAUDescriptor::VehicleToServerAUDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::VehicleToServerAU)), "inet::FieldsChunk")
{
    propertyNames = nullptr;
}

VehicleToServerAUDescriptor::~VehicleToServerAUDescriptor()
{
    delete[] propertyNames;
}

bool VehicleToServerAUDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<VehicleToServerAU *>(obj)!=nullptr;
}

const char **VehicleToServerAUDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *VehicleToServerAUDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int VehicleToServerAUDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 2+base->getFieldCount() : 2;
}

unsigned int VehicleToServerAUDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_AU
        FD_ISEDITABLE,    // FIELD_secret
    };
    return (field >= 0 && field < 2) ? fieldTypeFlags[field] : 0;
}

const char *VehicleToServerAUDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "AU",
        "secret",
    };
    return (field >= 0 && field < 2) ? fieldNames[field] : nullptr;
}

int VehicleToServerAUDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "AU") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "secret") == 0) return baseIndex + 1;
    return base ? base->findField(fieldName) : -1;
}

const char *VehicleToServerAUDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "string",    // FIELD_AU
        "string",    // FIELD_secret
    };
    return (field >= 0 && field < 2) ? fieldTypeStrings[field] : nullptr;
}

const char **VehicleToServerAUDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *VehicleToServerAUDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int VehicleToServerAUDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    VehicleToServerAU *pp = omnetpp::fromAnyPtr<VehicleToServerAU>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void VehicleToServerAUDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    VehicleToServerAU *pp = omnetpp::fromAnyPtr<VehicleToServerAU>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'VehicleToServerAU'", field);
    }
}

const char *VehicleToServerAUDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    VehicleToServerAU *pp = omnetpp::fromAnyPtr<VehicleToServerAU>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string VehicleToServerAUDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    VehicleToServerAU *pp = omnetpp::fromAnyPtr<VehicleToServerAU>(object); (void)pp;
    switch (field) {
        case FIELD_AU: return oppstring2string(pp->getAU());
        case FIELD_secret: return oppstring2string(pp->getSecret());
        default: return "";
    }
}

void VehicleToServerAUDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    VehicleToServerAU *pp = omnetpp::fromAnyPtr<VehicleToServerAU>(object); (void)pp;
    switch (field) {
        case FIELD_AU: pp->setAU((value)); break;
        case FIELD_secret: pp->setSecret((value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'VehicleToServerAU'", field);
    }
}

omnetpp::cValue VehicleToServerAUDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    VehicleToServerAU *pp = omnetpp::fromAnyPtr<VehicleToServerAU>(object); (void)pp;
    switch (field) {
        case FIELD_AU: return pp->getAU();
        case FIELD_secret: return pp->getSecret();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'VehicleToServerAU' as cValue -- field index out of range?", field);
    }
}

void VehicleToServerAUDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    VehicleToServerAU *pp = omnetpp::fromAnyPtr<VehicleToServerAU>(object); (void)pp;
    switch (field) {
        case FIELD_AU: pp->setAU(value.stringValue()); break;
        case FIELD_secret: pp->setSecret(value.stringValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'VehicleToServerAU'", field);
    }
}

const char *VehicleToServerAUDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr VehicleToServerAUDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    VehicleToServerAU *pp = omnetpp::fromAnyPtr<VehicleToServerAU>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void VehicleToServerAUDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    VehicleToServerAU *pp = omnetpp::fromAnyPtr<VehicleToServerAU>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'VehicleToServerAU'", field);
    }
}

Register_Class(ServerToVehicleAU)

ServerToVehicleAU::ServerToVehicleAU() : ::inet::FieldsChunk()
{
}

ServerToVehicleAU::ServerToVehicleAU(const ServerToVehicleAU& other) : ::inet::FieldsChunk(other)
{
    copy(other);
}

ServerToVehicleAU::~ServerToVehicleAU()
{
}

ServerToVehicleAU& ServerToVehicleAU::operator=(const ServerToVehicleAU& other)
{
    if (this == &other) return *this;
    ::inet::FieldsChunk::operator=(other);
    copy(other);
    return *this;
}

void ServerToVehicleAU::copy(const ServerToVehicleAU& other)
{
    this->AU = other.AU;
    this->secret = other.secret;
    this->rsuId = other.rsuId;
}

void ServerToVehicleAU::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::FieldsChunk::parsimPack(b);
    doParsimPacking(b,this->AU);
    doParsimPacking(b,this->secret);
    doParsimPacking(b,this->rsuId);
}

void ServerToVehicleAU::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::FieldsChunk::parsimUnpack(b);
    doParsimUnpacking(b,this->AU);
    doParsimUnpacking(b,this->secret);
    doParsimUnpacking(b,this->rsuId);
}

const char * ServerToVehicleAU::getAU() const
{
    return this->AU.c_str();
}

void ServerToVehicleAU::setAU(const char * AU)
{
    handleChange();
    this->AU = AU;
}

const char * ServerToVehicleAU::getSecret() const
{
    return this->secret.c_str();
}

void ServerToVehicleAU::setSecret(const char * secret)
{
    handleChange();
    this->secret = secret;
}

int ServerToVehicleAU::getRsuId() const
{
    return this->rsuId;
}

void ServerToVehicleAU::setRsuId(int rsuId)
{
    handleChange();
    this->rsuId = rsuId;
}

class ServerToVehicleAUDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_AU,
        FIELD_secret,
        FIELD_rsuId,
    };
  public:
    ServerToVehicleAUDescriptor();
    virtual ~ServerToVehicleAUDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(ServerToVehicleAUDescriptor)

ServerToVehicleAUDescriptor::ServerToVehicleAUDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::ServerToVehicleAU)), "inet::FieldsChunk")
{
    propertyNames = nullptr;
}

ServerToVehicleAUDescriptor::~ServerToVehicleAUDescriptor()
{
    delete[] propertyNames;
}

bool ServerToVehicleAUDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<ServerToVehicleAU *>(obj)!=nullptr;
}

const char **ServerToVehicleAUDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *ServerToVehicleAUDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int ServerToVehicleAUDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 3+base->getFieldCount() : 3;
}

unsigned int ServerToVehicleAUDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_AU
        FD_ISEDITABLE,    // FIELD_secret
        FD_ISEDITABLE,    // FIELD_rsuId
    };
    return (field >= 0 && field < 3) ? fieldTypeFlags[field] : 0;
}

const char *ServerToVehicleAUDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "AU",
        "secret",
        "rsuId",
    };
    return (field >= 0 && field < 3) ? fieldNames[field] : nullptr;
}

int ServerToVehicleAUDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "AU") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "secret") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "rsuId") == 0) return baseIndex + 2;
    return base ? base->findField(fieldName) : -1;
}

const char *ServerToVehicleAUDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "string",    // FIELD_AU
        "string",    // FIELD_secret
        "int",    // FIELD_rsuId
    };
    return (field >= 0 && field < 3) ? fieldTypeStrings[field] : nullptr;
}

const char **ServerToVehicleAUDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *ServerToVehicleAUDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int ServerToVehicleAUDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    ServerToVehicleAU *pp = omnetpp::fromAnyPtr<ServerToVehicleAU>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void ServerToVehicleAUDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    ServerToVehicleAU *pp = omnetpp::fromAnyPtr<ServerToVehicleAU>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'ServerToVehicleAU'", field);
    }
}

const char *ServerToVehicleAUDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    ServerToVehicleAU *pp = omnetpp::fromAnyPtr<ServerToVehicleAU>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string ServerToVehicleAUDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    ServerToVehicleAU *pp = omnetpp::fromAnyPtr<ServerToVehicleAU>(object); (void)pp;
    switch (field) {
        case FIELD_AU: return oppstring2string(pp->getAU());
        case FIELD_secret: return oppstring2string(pp->getSecret());
        case FIELD_rsuId: return long2string(pp->getRsuId());
        default: return "";
    }
}

void ServerToVehicleAUDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    ServerToVehicleAU *pp = omnetpp::fromAnyPtr<ServerToVehicleAU>(object); (void)pp;
    switch (field) {
        case FIELD_AU: pp->setAU((value)); break;
        case FIELD_secret: pp->setSecret((value)); break;
        case FIELD_rsuId: pp->setRsuId(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ServerToVehicleAU'", field);
    }
}

omnetpp::cValue ServerToVehicleAUDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    ServerToVehicleAU *pp = omnetpp::fromAnyPtr<ServerToVehicleAU>(object); (void)pp;
    switch (field) {
        case FIELD_AU: return pp->getAU();
        case FIELD_secret: return pp->getSecret();
        case FIELD_rsuId: return pp->getRsuId();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'ServerToVehicleAU' as cValue -- field index out of range?", field);
    }
}

void ServerToVehicleAUDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    ServerToVehicleAU *pp = omnetpp::fromAnyPtr<ServerToVehicleAU>(object); (void)pp;
    switch (field) {
        case FIELD_AU: pp->setAU(value.stringValue()); break;
        case FIELD_secret: pp->setSecret(value.stringValue()); break;
        case FIELD_rsuId: pp->setRsuId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ServerToVehicleAU'", field);
    }
}

const char *ServerToVehicleAUDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr ServerToVehicleAUDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    ServerToVehicleAU *pp = omnetpp::fromAnyPtr<ServerToVehicleAU>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void ServerToVehicleAUDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    ServerToVehicleAU *pp = omnetpp::fromAnyPtr<ServerToVehicleAU>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ServerToVehicleAU'", field);
    }
}

}  // namespace inet

namespace omnetpp {

}  // namespace omnetpp

