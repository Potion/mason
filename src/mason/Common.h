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

#pragma once

#include "mason/Mason.h"
#include "mason/Notifications.h"

#include "cinder/Filesystem.h"
#include "cinder/Log.h"
#include "cinder/System.h"
#include "cinder/AxisAlignedBox.h"
#include "mason/Mason.h"

#include "cinder/Rect.h"
#include "cinder/DataSource.h"
#include "cinder/Rand.h"
#include "cinder/Color.h"

#include <memory>
#include <string>

// TODO: remove, Hud handles this with a custom logger now
#define MA_LOG_EXCEPTION( str, exc )	{ CI_LOG_E( str << ", exception type: " << System::demangleTypeName( typeid( exc ).name() ) << ", what: " << exc.what() ); mason::NotificationCenter::post( mason::NOTIFY_ERROR ); }

namespace mason {

const ci::fs::path&		getRepoRootPath();
//! returns the directory that contains cinder.dart
ci::fs::path			getCinderDartDirectory();
//! returns the base directory for mason-specific dart code
ci::fs::path			getDartScriptDirectory();
//! returns the snapshot_gen.bin that ships with Cinder-Dart
ci::DataSourceRef		getDartSnapshot();
//! returns the root directory for common glsl
ci::fs::path			getGlslDirectory();
//! Returns the directory containing repo assets
ci::fs::path			getDataPath();

//! Initializes dart script paths and installs the snapshot bin according to mason repo layout.
void					initializeDartVM();

void    notifyResourceReloaded();

ci::Timeline*   timeline();

//! Initializes a bunch of global stuff
void		initialize();

// TODO: move these to mason/Rand.h
void		initRand( bool randomSeed = false );
ci::Rand*	rand();
ci::vec3	randVec3( const ci::AxisAlignedBox &box );
ci::Color	randColor();

bool epsilonEqual( const ci::Rectf &r1, const ci::Rectf &r2, float epsilon );

//! Returns a stringified stack trace ready for logging. TODO: move to cinder core
std::string stackTraceAsString( size_t startingFrame = 0, size_t count = 0, bool skipPlatformFrames = true );

} // namespace mason