/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <dirent.h>
#include <ctype.h>
#include "shutils.h"
#include "shared.h"

void run_custom_script(char *name, int timeout, char *arg1, char *arg2)
{
	char script[120];
	char *cmd[4];
	int pid;
	struct stat st;
	char *error;

	snprintf(script, sizeof(script), "/jffs/scripts/%s", name);

	if (!stat(script, &st)) {
		if (nvram_match("jffs2_scripts", "0"))
			error = "custom script execution is disabled!";
		else if (!(st.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)))
			error = "script is not set executable!";
		else
			error = NULL;

		if (error) {
			logmessage("custom_script", "Found %s, but %s", name, error);
			return;
		}

		if (arg1)
			logmessage("custom_script" ,"Running %s (args: %s%s%s)", script, arg1, (arg2 ? " " : ""), (arg2 ? arg2 : ""));
		else
			logmessage("custom_script" ,"Running %s", script);

		cmd[0] = script;
		cmd[1] = arg1;
		cmd[2] = arg2;
		cmd[3] = NULL;
		_eval( cmd, NULL, timeout, (timeout ? NULL : &pid));
	}
}
