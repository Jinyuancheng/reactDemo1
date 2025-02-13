/*
 *          Copyright Andrey Semashev 2007 - 2015.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 */
 /*!
  * \file   main.cpp
  * \author Andrey Semashev
  * \date   11.11.2007
  *
  * \brief  An example of basic library usage. See the library tutorial for expanded
  *         comments on this code. It may also be worthwhile reading the Wiki requirements page:
  *         http://www.crystalclearsoftware.com/cgi-bin/boost_wiki/wiki.pl?Boost.Logging
  */

  // #define BOOST_LOG_USE_CHAR
  // #define BOOST_ALL_DYN_LINK 1
  // #define BOOST_LOG_DYN_LINK 1

#include <iostream>

#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>

#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/attributes/timer.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/support/date_time.hpp>

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

using boost::shared_ptr;

// Here we define our application severity levels.
enum severity_level
{
	normal,
	notification,
	warning,
	error,
	critical
};

// The formatting logic for the severity level
template< typename CharT, typename TraitsT >
inline std::basic_ostream< CharT, TraitsT >& operator<< (
	std::basic_ostream< CharT, TraitsT >& strm, severity_level lvl)
{
	static const char* const str[] =
	{
		"normal",
		"notification",
		"warning",
		"error",
		"critical"
	};
	/*\ 使用static_cast 进行强制类型转换 \*/
	if (static_cast<std::size_t>(lvl) < (sizeof(str) / sizeof(*str)))
		strm << str[lvl];
	else
		strm << static_cast<int>(lvl);
	return strm;
}

int main(int argc, char* argv[])
{
	// This is a simple tutorial/example of Boost.Log usage

	// The first thing we have to do to get using the library is
	// to set up the logging sinks - i.e. where the logs will be written to.
	/*\ 将数据输出到终端 \*/
	logging::add_console_log(std::clog, keywords::format = "%TimeStamp%: %Message%");

	// One can also use lambda expressions to setup filters and formatters
	/*\ 将日志写入到文件中 \*/
	logging::add_file_log
	(
		"sample.log",
		/*\ 设置筛选器 \*/
		keywords::filter = expr::attr< severity_level >("Severity") >= severity_level::warning,
		/*keywords::format = expr::stream
		<< expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d, %H:%M:%S.%f")
		<< " [" << expr::format_date_time< attrs::timer::value_type >("Uptime", "%O:%M:%S")
		<< "] [" << expr::format_named_scope("Scope", keywords::format = "%n (%f:%l)")
		<< "] <" << expr::attr< severity_level >("Severity")
		<< "> " << expr::message*/
		/*\ 设置日志的格式 \*/
		keywords::format = expr::format("%1% [%2%] [%3%] <%4%> %5%")
		% expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d, %H:%M:%S.%f")
		% expr::format_date_time< attrs::timer::value_type >("Uptime", "%O:%M:%S")
		% expr::format_named_scope("Scope", keywords::format = "%n (%f:%l)")
		% expr::attr< severity_level >("Severity")
		% expr::message

	);

	// Also let's add some commonly used attributes, like timestamp and record counter.
	logging::add_common_attributes();
	logging::core::get()->add_thread_attribute("Scope", attrs::named_scope());

	BOOST_LOG_FUNCTION();

	// Now our logs will be written both to the console and to the file.
	// Let's do a quick test and output something. We have to create a logger for this.
	src::logger lg;

	// And output...
	BOOST_LOG(lg) << "Hello, World!";

	// Now, let's try logging with severity
	src::severity_logger< severity_level > slg;

	// Let's pretend we also want to profile our code, so add a special timer attribute.
	slg.add_attribute("Uptime", attrs::timer());

	BOOST_LOG_SEV(slg, normal) << "A normal severity message, will not pass to the file";
	BOOST_LOG_SEV(slg, warning) << "A warning severity message, will pass to the file";
	BOOST_LOG_SEV(slg, error) << "An error severity message, will pass to the file";

	return 0;
}
