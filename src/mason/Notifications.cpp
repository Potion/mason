/*
 Copyright (c) 2014, Richard Eakin - All rights reserved.

 Redistribution and use in source and binary forms, with or without modification, are permitted provided
 that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, this list of conditions and
 the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
 the following disclaimer in the documentation and/or other materials provided with the distribution.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
*/

#include "mason/Notifications.h"

// Dispatch:
#include "cinder/app/App.h"
#include "cinder/Timeline.h"

using namespace ci;
using namespace std;

namespace mason {

// ----------------------------------------------------------------------------------------------------
// NotificationCenter
// ----------------------------------------------------------------------------------------------------

const char* NOTIFY_RESOURCE_RELOADED	= "NOTIFY_RESOURCE_RELOADED";
const char* NOTIFY_ERROR				= "NOTIFY_ERROR";

// static
NotificationCenter* NotificationCenter::instance()
{
	static NotificationCenter sInstance;
	return &sInstance;
}

void NotificationCenter::postImpl( const char *name, const Notification &notification )
{
	mRegisteredNotificiations[name].emit( notification );
}

void NotificationCenter::listenImpl( const char *name, const NotificationCallback &slot )
{
	mRegisteredNotificiations[name].connect( slot );
}

// ----------------------------------------------------------------------------------------------------
// Dispatch
// ----------------------------------------------------------------------------------------------------

namespace {
	ci::TimelineRef sTimeline;
}

// static
void Dispatch::setTimeline( const ci::TimelineRef &timeline )
{
	sTimeline = timeline;
}

// static
ci::Timeline* Dispatch::getTimeline()
{
	CI_ASSERT( sTimeline );

	if( sTimeline )
		return sTimeline.get();
	else
		return &app::timeline();
}

void Dispatch::once( double delaySeconds, const std::function<void ()> &func )
{
	auto timeline = getTimeline();

	// Ahah. this was correcting the timeline for the dispatch, but it was too late for updateCurrentTime()
	// - perhaps I should be using a Timeline controlled by Score, and do the stepTo there
//	timeline.stepTo( app::getElapsedSeconds() );

	timeline->add( func, timeline->getCurrentTime() + delaySeconds );
}

// static
void Dispatch::onMain( const std::function<void ()> &func )
{
	if( app::isMainThread() )
		func();
	else
		app::App::get()->dispatchAsync( func );
}



// ----------------------------------------------------------------------------------------------------
// LoggerNotification
// ----------------------------------------------------------------------------------------------------

void LoggerNotification::write( const ci::log::Metadata &meta, const std::string &text )
{
	if( meta.mLevel >= log::Level::LEVEL_ERROR ) {
		NotificationCenter::post( NOTIFY_ERROR );
	}
}

} // namespace mason
