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

#include "ui.h"



Backend backend;



int main (
	int			const	argc,
	char	const * const * const	argv
	)
{
	if ( 0 == backend.command_line ( argc, argv ) )
	{
		backend.start_ui ();
	}

	return backend.exit.value ();
}



#include <readline/readline.h>
#include <readline/history.h>



Backend::Backend ()
	:
	m_conf_path	()
{
	m_random.set_seed_by_time ();
}

Backend::~Backend ()
{
}

static int error_func (
	int		const	error,
	int		const	arg,
	int		const	argc,
	char	const * const	argv[],
	void		* const	ARG_UNUSED ( user_data )
	)
{
	std::cerr << "error_func: Argument ERROR! -"
		<< " num=" << error
		<< " arg=" << arg
		<< "/" << (argc - 1);

	if ( arg < argc )
	{
		std::cerr << " '" << argv[arg] << "'";
	}

	std::cerr << "\n";

	return 0;		// Keep parsing
}

static int init_table ( mtKit::CliTab &clitab )
{
	if (	0
		|| clitab.add_item ( "about", jtf_about )

		|| clitab.add_item ( "butt add buckets", jtf_butt_add_buckets,
			1, 1,"<INTEGER>")
		|| clitab.add_item ( "butt add name", jtf_butt_add_name, 1, 1,
			"<STRING>" )
		|| clitab.add_item ( "butt info", jtf_butt_info )
		|| clitab.add_item ( "butt list", jtf_butt_list )
		|| clitab.add_item ( "butt set name", jtf_butt_set_name, 1, 1,
			"<STRING>" )

		|| clitab.add_item ( "help", jtf_help, 0, 100, "[ARG]..." )
		|| clitab.add_item ( "q", jtf_quit )
		|| clitab.add_item ( "quit", jtf_quit )

		|| clitab.add_item ( "soda decode", jtf_soda_decode, 2, 2,
			"<INPUT FILENAME> <OUTPUT FILENAME>")
		|| clitab.add_item ( "soda encode", jtf_soda_encode, 2, 2,
			"<INPUT FILENAME> <OUTPUT FILENAME>" )
		|| clitab.add_item ( "soda file info", jtf_soda_file_info, 1, 1,
			"<FILENAME>" )
		|| clitab.add_item ( "soda info", jtf_soda_info )
		|| clitab.add_item ( "soda multi decode", jtf_soda_multi_decode,
			2, 2, "<INPUT FILENAME> <OUTPUT FILENAME>" )
		|| clitab.add_item ( "soda multi encode", jtf_soda_multi_encode,
			4, 18,
			"<INPUT FILENAME> <OUTPUT FILENAME> <BUTT NAME>..." )
		|| clitab.add_item ( "soda set mode", jtf_soda_set_mode, 1, 1,
			"<INTEGER>" )

		|| clitab.add_item ( "tap decode", jtf_tap_decode, 2, 2,
			"<INPUT FILENAME> <OUTPUT FILENAME>" )
		|| clitab.add_item ( "tap encode", jtf_tap_encode, 3, 3,
			"<INPUT BOTTLE> <INPUT FILE> <OUTPUT BOTTLE>" )
		|| clitab.add_item ( "tap file info", jtf_tap_file_info, 1, 1,
			"<FILENAME>" )
		|| clitab.add_item ( "tap multi decode", jtf_tap_multi_decode,
			2, 2, "<INPUT FILENAME> <OUTPUT FILENAME>" )

		|| clitab.add_item ( "well add path", jtf_well_add_path, 1, 1,
			"<PATH>" )
		|| clitab.add_item ( "well empty", jtf_well_empty )
		|| clitab.add_item ( "well info", jtf_well_info )
		|| clitab.add_item ( "well save file", jtf_well_save_file, 2, 2,
			"<BYTES> <FILENAME>" )
		|| clitab.add_item ( "well seed", jtf_well_seed )
		|| clitab.add_item ( "well seed int", jtf_well_seed_int, 1, 1,
			"<INTEGER>" )
		)
	{
		return 1;
	}

	return 0;
}

void Backend::main_loop ()
{
	if ( init_table ( m_clitab ) )
	{
		exit.abort ();
		exit.set_value ( 1 );

		std::cerr << "Error: main_loop().init_table()\n";

		return;
	}

	while ( false == exit.aborted () )
	{
		char * line = readline ( APP_NAME" > " );

		if ( ! line )
		{
			break;
		}

		if ( line[0] )
		{
			add_history ( line );
		}

		if ( line[0] == '#' )
		{
			// Comment
		}
		else
		{
			m_clitab.parse ( line );
		}

		free ( line );
		line = NULL;
	}

	clear_history ();
}

void Backend::start_ui ()
{
	try
	{
		m_well.reset ( new mtDW::Well ( m_conf_path ) );
		m_butt.reset ( new mtDW::Butt ( m_random, m_conf_path ) );
		m_soda.reset ( new mtDW::Soda ( m_conf_path ) );
		m_tap.reset ( new mtDW::Tap () );

		main_loop ();
	}
	catch ( ... )
	{
		exit.set_value ( 1 );
		exit.abort ();
	}
}

int Backend::command_line (
	int			const	argc,
	char	const * const * const	argv
	)
{
	m_conf_path = NULL;

	int	show_version	= 0;
	int	show_about	= 1;
	int	tab_text	= 0;

	mtArg	const	arg_list[] = {
	{ "-help",	MTKIT_ARG_SWITCH, &show_version, 2, NULL },
	{ "-version",	MTKIT_ARG_SWITCH, &show_version, 1, NULL },
	{ "conf",	MTKIT_ARG_STRING, &m_conf_path, 0, NULL },
	{ "q",		MTKIT_ARG_SWITCH, &show_about, 0, NULL },
	{ "t",		MTKIT_ARG_SWITCH, &tab_text, 1, NULL },
	{ NULL, 0, NULL, 0, NULL }
	};


	mtkit_arg_parse( argc, argv, arg_list, NULL, error_func, NULL);

	if ( show_version )
	{
		printf ( "%s (part of %s)\n\n", argv[0], VERSION );

		if ( 2 == show_version )
		{
			printf (
				"For further information consult the man page "
				"%s(1) or the mtCellEdit Handbook.\n"
				"\n", argv[0] );
		}

		return 1;		// Quit program
	}

	if ( show_about )
	{
		jtf_about ( NULL );
	}

	rl_variable_bind ( "expand-tilde", "on" );

	if ( tab_text )
	{
		rl_bind_key ( '\t', rl_insert );
	}

	return 0;			// Continue program
}

int Backend::get_help (
	char	const * const * const	argv
	) const
{
	return m_clitab.print_help ( argv );
}

