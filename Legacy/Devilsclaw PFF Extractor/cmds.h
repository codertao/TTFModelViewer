/***************************************************************************
 *   Copyright (C) 2007 by Neil Hellfeldt   *
 *   devilsclaw@devilsclaws.net   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef CMDS_H
#define CMDS_H
void cmd_verbose(void* Input);
void cmd_debug(void* Input);
void cmd_version(void* Input);
void cmd_help(void* Input);
void cmd_list(void* Input);
void cmd_pfftype(void* Input);
void cmd_create(void* Input);
void cmd_compress(void* Input);
void cmd_insert(void* Input);
void cmd_compall(void* Input);
void cmd_outdir(void* Input);
void cmd_decompress(void* Input);
void cmd_extract(void* Input);
void cmd_decompall(void* Input);
void cmd_merge(void* Input);
void cmd_markdeleted(void* Input);
void cmd_optimize(void* Input);
#endif
