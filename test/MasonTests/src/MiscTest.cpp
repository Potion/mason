#include "MiscTest.h"

#include "cinder/gl/gl.h"
#include "cinder/Log.h"
#include "cinder/CinderAssert.h"
#include "cinder/app/App.h"

#include "mason/Mason.h"
#include "jsoncpp/json.h"

using namespace ci;
using namespace std;

MiscTest::MiscTest()
{
	auto nbox = make_shared<ui::NumberBox>( Rectf( 200, 360, 280, 400 ) );
	nbox->setTitle( "val" );
	//nbox->setBackgroundEnabled( false );
	addSubview( nbox );

	auto nbox3 = make_shared<ui::NumberBox3>( Rectf( 200, 410, 380, 450 ) );
	ui::NumberBox3* nbox3Ptr = nbox3.get();
//	nbox3->getSignalValueChanged().connect( [nbox3Ptr] { CI_LOG_I( "nbox3 value: " << nbox3Ptr->getValue(); ); } );

	addSubview( nbox3 );


	fs::path jsonFileName = "dictionary_test.json";
	try {
		mWatchDict = ma::FileWatcher::load( jsonFileName, [this] ( const fs::path &filePath ) {
			try {
				auto dict = ma::Dictionary::convert<Json::Value>( loadFile( filePath ) );
				testDict( dict );

				CI_LOG_I( "testDict completed." );
			}
			catch( exception &exc ) {
				CI_LOG_EXCEPTION( "failed to load Dictionary", exc );
			}
		} );
	}
	catch( exception &exc ) {
		CI_LOG_EXCEPTION( "failed to load Dictionary", exc );
	}
}

void MiscTest::testDict( const ma::Dictionary &dict )
{
	CI_LOG_I( "number of elements: " << dict.getSize() );

	int a = dict.get<int>( "a" );
	CI_LOG_I( "a: " << a );
	float b = dict.get<float>( "b" );
	CI_LOG_I( "b: " << b );
	string c = dict.get<string>( "c" );
	CI_LOG_I( "c: " << c );

	// nested Dictionary, copied
	auto nested = dict.get<ma::Dictionary>( "nested" );
	CI_LOG_I( "nested blah string: " << nested.get<string>( "blah" ) );

	// nested Dictionary without copying
	const auto &nestedRef = dict.getStrict<ma::Dictionary>( "nested" );
	CI_LOG_I( "nestedRef days: " << nestedRef.get<int>( "days" ) );

	// vector with mixed types
	auto mixedArray = dict.get<vector<boost::any>>( "mixedArray" );
	string typesStr;
	for( size_t i = 0; i < mixedArray.size(); i++ ) {
		typesStr += "[" + to_string( i ) + "] " + System::demangleTypeName( mixedArray[i].type().name() ) + ", ";
	}
	CI_LOG_I( "num elements in mixedArray: " << mixedArray.size() << "', types: " << typesStr );

	// vector with uniform elemnt types
	auto oddNumbers = dict.get<vector<int>>( "oddNumbers" );
	string oddNumbersStr;
	for( size_t i = 0; i < oddNumbers.size(); i++ ) {
		oddNumbersStr += "[" + to_string( i ) + "] " + to_string( oddNumbers[i] ) + ", ";
	}
	CI_LOG_I( "num elements in oddNumbers: " << oddNumbers.size() << ", values: " << oddNumbersStr );
}

void MiscTest::layout()
{
}

void MiscTest::update()
{
}

void MiscTest::draw( ui::Renderer *ren )
{
	gl::ScopedColor col( Color( 0.3f, 0.1f, 0 ) );
	gl::drawSolidRect( Rectf( 0, 0, getWidth(), getHeight() ) );
}
