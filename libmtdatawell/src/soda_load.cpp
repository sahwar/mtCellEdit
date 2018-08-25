/*
	Copyright (C) 2018 Mark Tyler

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program in the file COPYING.
*/

#include "soda.h"



mtDW::SodaFile::SodaFile ()
	:
	m_mode_raw	(0),
	m_filesize	(0),
	m_bucket_pos	(0),
	m_bucket	(0)
{
}

mtDW::SodaFile::~SodaFile ()
{
}



struct UtreeLoad
{
	UtreeLoad (
		uint8_t		* const	mem,
		uint32_t	const	size
		)
	{
		m_root = mtkit_utree_load_mem ( NULL, (char *)mem, size, NULL );

		m_soda = mtkit_utree_get_node ( m_root, UTREE_ROOT_NAME,
			MTKIT_UTREE_NODE_TYPE_ELEMENT );

		free ( mem );
	}

	~UtreeLoad ()
	{
		mtkit_utree_destroy_node ( m_root );
		m_root = NULL;
	}

	void get_int ( int & num, char const * const name )
	{
		mtkit_utree_get_attribute_int ( m_soda, name, & num );
	}

	void get_string ( std::string & str, char const * const name )
	{
		char const * val;

		if ( 0 == mtkit_utree_get_attribute_str ( m_soda, name, & val ))
		{
			str = val;
		}
	}

	void get_uint64 ( uint64_t & num, char const * const name )
	{
		char const * val;

		if ( 0 == mtkit_utree_get_attribute_str ( m_soda, name, & val ))
		{
			sscanf ( val, "%" PRIu64, &num );
		}
	}

/// ----------------------------------------------------------------------------

	mtUtreeNode  	* m_root;
	mtUtreeNode  	* m_soda;
};



int mtDW::SodaFile::open ( char const * const filename )
{
	char id[ mtKit::ChunkFile::CHUNK_HEADER_SIZE ] = {0};

	if ( m_chunk.open ( filename, id ) )
	{
		return -1;
	}

	if ( 0 != memcmp ( id, FILE_ID, sizeof(id) ) )
	{
		std::cerr << "Invalid Soda file ID.\n";
		return 1;
	}

	uint8_t * buf;
	uint32_t buflen;

	if ( m_chunk.get_chunk ( &buf, &buflen, id, NULL ) )
	{
		std::cerr << "Unable to read first chunk.\n";
		return 1;
	}

	if ( 0 != memcmp ( id, FILE_CHUNK_ID, sizeof(id) ) )
	{
		std::cerr << "Invalid Soda file header chunk ID.\n";
		return 1;
	}

	UtreeLoad utree ( buf, buflen );
	if ( ! utree.m_soda )
	{
		std::cerr << "Unable to find Soda header.\n";
		return 1;
	}

	utree.get_int ( m_mode_raw, HEADER_ITEM_MODE );
	utree.get_uint64 ( m_filesize, HEADER_ITEM_SIZE );

	if ( ! m_mode_raw )
	{
		utree.get_int ( m_bucket_pos, HEADER_ITEM_POS );
		utree.get_int ( m_bucket, HEADER_ITEM_BUCKET );
		utree.get_string ( m_butt_name, HEADER_ITEM_BUTT );
	}

	return 0;
}

