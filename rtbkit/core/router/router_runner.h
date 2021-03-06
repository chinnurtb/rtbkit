/** router_runner.h                                                -*- C++ -*-
    Jeremy Barnes, 17 December 2012
    Copyright (c) 2012 Datacratic.  All rights reserved.

    Program to run the router.
*/

#pragma once


#include <boost/program_options/options_description.hpp>
#include "rtbkit/core/router/router.h"
#include "rtbkit/core/banker/slave_banker.h"
#include "soa/service/service_utils.h"

namespace RTBKIT {


/*****************************************************************************/
/* ROUTER RUNNER                                                             */
/*****************************************************************************/

struct RouterRunner {

    RouterRunner();

    ServiceProxyArguments serviceArgs;
    std::vector<std::string> logUris;  ///< TODO: zookeeper

    std::string exchangeConfigurationFile;
    std::string bidderConfigurationFile;

    float lossSeconds;
    bool noPostAuctionLoop;

    bool logAuctions;
    bool logBids;

    float maxBidPrice;

    void doOptions(int argc, char ** argv,
                   const boost::program_options::options_description & opts
                   = boost::program_options::options_description());

    std::shared_ptr<ServiceProxies> proxies;
    std::shared_ptr<SlaveBanker> banker;
    std::shared_ptr<Router> router;
    Json::Value exchangeConfig;
    Json::Value bidderConfig;

    void init();

    void start();

    void shutdown();

};

} // namespace RTBKIT

