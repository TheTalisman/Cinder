/*
 Copyright (c) 2014, The Cinder Project

 This code is intended to be used with the Cinder C++ library, http://libcinder.org

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this list of conditions and
 the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
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

#include "cinder/audio/Source.h"

//! don't include ogg's static callbacks (we rely on cinder's stream utils instead)
#define OV_EXCLUDE_STATIC_CALLBACKS

#include "vorbis/codec.h"
#include "vorbis/vorbisfile.h"

namespace cinder { namespace audio {

namespace dsp {
	class Converter;
}

//! SourceFile implementation for decoding ogg vorbis files.
class SourceFileOggVorbis : public SourceFile {
  public:
	SourceFileOggVorbis();
	SourceFileOggVorbis( const DataSourceRef &dataSource, size_t sampleRate );
	virtual ~SourceFileOggVorbis();

	SourceFileRef	cloneWithSampleRate( size_t sampleRate ) const	override;

	size_t		getNumChannels() const	override		{ return mNumChannels; }
	size_t		getSampleRateNative() const	override	{ return mSampleRate; }

	size_t		performRead( Buffer *buffer, size_t bufferFrameOffset, size_t numFramesNeeded )		override;
	void		performSeek( size_t readPositionFrames )											override;
	std::string getMetaData() const																	override;

  private:
	void init();
	void setupConversionImpl();

	// ov_callbacks
	static size_t	readFn( void *ptr, size_t size, size_t count, void *datasource );
	static int		seekFn( void *datasource, ogg_int64_t offset, int whence );
	static int		closeFn( void *datasource );
	static long		tellFn( void *datasource );

	::OggVorbis_File	mOggVorbisFile;

	ci::DataSourceRef	mDataSource;
	ci::IStreamRef		mStream;
	size_t				mNumChannels, mSampleRate;
};

//class TargetFileImplOggVorbis : public TargetFile {
//public:
//	TargetFileImplOggVorbis( const DataTargetRef &dataTarget, size_t sampleRate, size_t numChannels, const std::string &extension );
//	virtual TargetFileImplOggVorbis() {}
//
//	void write( const Buffer *buffer, size_t frameOffset, size_t numFrames ) override;
//
//private:
//};

} } // namespace cinder::audio
