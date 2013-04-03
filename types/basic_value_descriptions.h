/* basic_value_descriptions.h                                      -*- C++ -*-
   Jeremy Barnes, 20 March 2013
   Copyright (c) 2013 Datacratic Inc.  All rights reserved.

*/

#pragma once

#include "value_description.h"
#include "soa/types/url.h"
#include "soa/types/date.h"


namespace Datacratic {

/*****************************************************************************/
/* DEFAULT DESCRIPTIONS FOR BASIC TYPES                                      */
/*****************************************************************************/

template<>
struct DefaultDescription<Datacratic::Id>
    : public ValueDescriptionI<Datacratic::Id> {

    virtual void parseJsonTyped(Datacratic::Id * val,
                                JsonParsingContext & context) const
    {
        Datacratic::parseJson(val, context);
    }

    virtual void printJsonTyped(const Datacratic::Id * val,
                                JsonPrintingContext & context) const
    {
        context.writeString(val->toString());
    }

    virtual bool isDefaultTyped(const Datacratic::Id * val) const
    {
        return !val->notNull();
    }
};

template<>
struct DefaultDescription<std::string>
    : public ValueDescriptionI<std::string> {

    virtual void parseJsonTyped(std::string * val,
                                JsonParsingContext & context) const
    {
        *val = context.expectStringAscii();
    }

    virtual void printJsonTyped(const std::string * val,
                                JsonPrintingContext & context) const
    {
        context.writeString(*val);
    }

    virtual bool isDefaultTyped(const std::string * val) const
    {
        return val->empty();
    }
};

template<>
struct DefaultDescription<Utf8String>
    : public ValueDescriptionI<Utf8String> {

    virtual void parseJsonTyped(Utf8String * val,
                                JsonParsingContext & context) const
    {
        *val = context.expectStringUtf8();
    }

    virtual void printJsonTyped(const Utf8String * val,
                                JsonPrintingContext & context) const
    {
        context.writeStringUtf8(*val);
    }

    virtual bool isDefaultTyped(const Utf8String * val) const
    {
        return val->empty();
    }
};

template<>
struct DefaultDescription<Url>
    : public ValueDescriptionI<Url> {

    virtual void parseJsonTyped(Url * val,
                                JsonParsingContext & context) const
    {
        *val = Url(context.expectStringUtf8());
    }

    virtual void printJsonTyped(const Url * val,
                                JsonPrintingContext & context) const
    {
        context.writeStringUtf8(val->toUtf8String());
    }

    virtual bool isDefaultTyped(const Url * val) const
    {
        return val->empty();
    }
};

template<>
struct DefaultDescription<int>
    : public ValueDescriptionI<int> {

    virtual void parseJsonTyped(int * val,
                                JsonParsingContext & context) const
    {
        *val = context.expectInt();
    }
    
    virtual void printJsonTyped(const int * val,
                                JsonPrintingContext & context) const
    {
        context.writeInt(*val);
    }
};

template<>
struct DefaultDescription<float>
    : public ValueDescriptionI<float> {

    virtual void parseJsonTyped(float * val,
                                JsonParsingContext & context) const
    {
        *val = context.expectFloat();
    }

    virtual void printJsonTyped(const float * val,
                                JsonPrintingContext & context) const
    {
        context.writeFloat(*val);
    }
};

template<typename T>
struct DefaultDescription<std::vector<T> >
    : public ValueDescriptionI<std::vector<T> > {
};

template<typename T>
struct DefaultDescription<std::unique_ptr<T> >
    : public ValueDescriptionI<std::unique_ptr<T> > {

    DefaultDescription(ValueDescriptionT<T> * inner
                       = getDefaultDescription((T *)0))
        : inner(inner)
    {
    }

    std::unique_ptr<ValueDescriptionT<T> > inner;

    virtual void parseJsonTyped(std::unique_ptr<T> * val,
                                JsonParsingContext & context) const
    {
        val->reset(new T());
        inner->parseJsonTyped(val->get(), context);
    }

    virtual void printJsonTyped(const std::unique_ptr<T> * val,
                                JsonPrintingContext & context) const
    {
        if (!val->get())
            context.skip();
        else inner->printJsonTyped(val->get(), context);
    }

    virtual bool isDefaultTyped(const std::unique_ptr<T> * val) const
    {
        return !val->get();
    }
};

template<>
struct DefaultDescription<Json::Value>
    : public ValueDescriptionI<Json::Value> {

    virtual void parseJsonTyped(Json::Value * val,
                                JsonParsingContext & context) const
    {
        Datacratic::parseJson(val, context);
    }

    virtual void printJsonTyped(const Json::Value * val,
                                JsonPrintingContext & context) const
    {
        context.writeJson(*val);
    }

    virtual bool isDefaultTyped(const Json::Value * val) const
    {
        return val->isNull();
    }
};

template<>
struct DefaultDescription<bool>
    : public ValueDescriptionI<bool> {

    virtual void parseJsonTyped(bool * val,
                                JsonParsingContext & context) const
    {
        *val = context.expectBool();
    }

    virtual void printJsonTyped(const bool * val,
                                JsonPrintingContext & context) const
    {
        context.writeBool(*val);
    }

    virtual bool isDefaultTyped(const bool * val) const
    {
        return false;
    }
};

template<>
struct DefaultDescription<Date>
    : public ValueDescriptionI<Date> {

    virtual void parseJsonTyped(Date * val,
                                JsonParsingContext & context) const
    {
        if (context.isNumber())
            *val = Date::fromSecondsSinceEpoch(context.expectDouble());
        else if (context.isString())
            *val = Date::parseDefaultUtc(context.expectStringAscii());
        else context.exception("expected date");
    }

    virtual void printJsonTyped(const Date * val,
                                JsonPrintingContext & context) const
    {
        context.writeJson(val->secondsSinceEpoch());
    }

    virtual bool isDefaultTyped(const Date * val) const
    {
        return *val == Date();
    }
};

} // namespace Datacratic
