/*
 *	 /home/fx/workspace/gitted/cmake/sql/src/sqlJobs/sqlJobs.cpp
 *	 Generated by sql2class v1.9.3 by (C) AH 2000-2006
 *	  using command line
 *	  $ sql2class -sqlite -build -global -prefix /home/fx/workspace/gitted/cmake/sql -lib sqlJobs -namespace db -overwrite dump
 *	 Date: Sat Nov 20 09:19:55 2010
 */

/*
Copyright (C) 2001-2006  Anders Hedstrom (grymse@alhem.net)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#include "ADM_default.h"
#include "sqlJobs.h"

namespace db {

/**
 **  Begin class 'Version'
 **/

Version::Version(Database *db)
{
	database = db;
	new_object = 1;
	clear();
}


Version::Version(Database *db,const std::string& sql)
{
	database = db;
	new_object = 1;
	spawn(sql);
}


Version::Version(Database *db,Query *qd,int offset)
{
	database = db;
	new_object = 0;
	spawn(qd, offset);
}


Version::~Version()
{
}


void Version::select(const std::string& sql)
{
	spawn(sql);
}


sqlite_int64 Version::insert()
{
	Query q(*database);
	std::string sql;

	sql = "insert into version(value)";
	{
		char slask[100];
		sprintf(slask," values(%ld",this -> value);
		sql += slask;
	}
	sql += ")";
	q.execute(sql);
	new_object = 0;
	sqlite_int64 inserted_id = q.insert_id();
	this->value = (long)inserted_id;
	return inserted_id;
}


void Version::save()
{
	if (new_object)
		insert();
}


std::string Version::xml()
{
	Query q(*database);
	std::string dest;
	char slask[200];
	dest = "<VERSION>";
	sprintf(slask,"<VALUE>%ld</VALUE>",this -> value);
	dest += slask;
	dest += "</VERSION>";
	return dest;
}


std::string Version::xml(const std::string& tag,const std::string& xvalx)
{
	Query q(*database);
	std::string dest;
	char slask[200];
	dest = "<VERSION " + tag + "=\"" + xvalx + "\">";
	sprintf(slask,"<VALUE>%ld</VALUE>",this -> value);
	dest += slask;
	dest += "</VERSION>";
	return dest;
}


size_t Version::num_cols()
{
	return 1;
}


void Version::clear()
{
	this -> value = 0;
}


void Version::spawn(const std::string& sql)
{
	Query q(*database);
	std::string temp;

	clear();

	if (!strncasecmp(sql.c_str(),"select * ",9))
	{
		temp = "select value " + sql.substr(9);
	} else
		temp = sql;
	q.get_result(temp);
	if (q.fetch_row())
	{
		this -> value = q.getval(0);																				// 0 - value integer
		new_object = 0;
	} else
		clear();
	q.free_result();
}


void Version::spawn(Query *qd,int offset)
{
	clear();

	this -> value = qd -> getval(0 + offset);																				// 0 - value integer
}


// End of implementation of class 'Version'

} // End of namespace
namespace db {

/**
 **  Begin class 'Jobs'
 **/

Jobs::Jobs(Database *db)
{
	database = db;
	new_object = 1;
	clear();
}


Jobs::Jobs(Database *db,const std::string& sql)
{
	database = db;
	new_object = 1;
	spawn(sql);
}


Jobs::Jobs(Database *db,Query *qd,int offset)
{
	database = db;
	new_object = 0;
	spawn(qd, offset);
}


Jobs::Jobs(Database& db,long i_id):database(&db),new_object(1)
{
	Query q(*database);
	std::string sql = "select * from jobs where ";
	{
		char slask[100];
		sprintf(slask,"id='%ld'",i_id);
		sql += slask;
	}
	spawn(sql);
}


Jobs::~Jobs()
{
}


void Jobs::select(const std::string& sql)
{
	spawn(sql);
}


sqlite_int64 Jobs::insert()
{
	Query q(*database);
	std::string sql;

	sql = "insert into jobs(jscript,jobname,outputFile,status,startTime,endTime)";
	sql += " values('" + q.GetDatabase().safestr(this -> jscript) + "'";
	sql += ", '" + q.GetDatabase().safestr(this -> jobname) + "'";
	sql += ", '" + q.GetDatabase().safestr(this -> outputfile) + "'";
	{
		char slask[100];
		sprintf(slask,", %ld",this -> status);
		sql += slask;
	}
	{
		char slask[100];
		sprintf(slask,", %ld",this -> starttime);
		sql += slask;
	}
	{
		char slask[100];
		sprintf(slask,", %ld",this -> endtime);
		sql += slask;
	}
	sql += ")";
	q.execute(sql);
	new_object = 0;
	sqlite_int64 inserted_id = q.insert_id();
	id = (long)inserted_id;
	return inserted_id;
}


void Jobs::update()
{
	update(this -> id);
}


void Jobs::update(long i_id)
{
	Query q(*database);
	std::string sql;
	sql += "update jobs set jscript='" + q.GetDatabase().safestr(this -> jscript) + "'";
	sql += ", jobname='" + q.GetDatabase().safestr(this -> jobname) + "'";
	sql += ", outputFile='" + q.GetDatabase().safestr(this -> outputfile) + "'";
	{
		char slask[200];
		sprintf(slask,", status=%ld",this -> status);
		sql += slask;
	}
	{
		char slask[200];
		sprintf(slask,", startTime=%ld",this -> starttime);
		sql += slask;
	}
	{
		char slask[200];
		sprintf(slask,", endTime=%ld",this -> endtime);
		sql += slask;
	}
	{
		char slask[200];
		sprintf(slask," where id='%ld'",i_id);
		sql += slask;
	}
	q.execute(sql);
}


void Jobs::save()
{
	if (new_object)
		insert();
	else
		update();
}


void Jobs::erase()
{
	if (!new_object)
	{
		std::string sql = "delete from jobs where";
		Query q(*database);
		{
			char slask[200];
			sprintf(slask," id='%ld'",this -> id);
			sql += slask;
		}
		q.execute(sql);
	}
}


std::string Jobs::xml()
{
	Query q(*database);
	std::string dest;
	char slask[200];
	dest = "<JOBS>";
	sprintf(slask,"<ID>%ld</ID>",this -> id);
	dest += slask;
	dest += "<JSCRIPT>" + q.GetDatabase().xmlsafestr(this -> jscript) + "</JSCRIPT>";
	dest += "<JOBNAME>" + q.GetDatabase().xmlsafestr(this -> jobname) + "</JOBNAME>";
	dest += "<OUTPUTFILE>" + q.GetDatabase().xmlsafestr(this -> outputfile) + "</OUTPUTFILE>";
	sprintf(slask,"<STATUS>%ld</STATUS>",this -> status);
	dest += slask;
	sprintf(slask,"<STARTTIME>%ld</STARTTIME>",this -> starttime);
	dest += slask;
	sprintf(slask,"<ENDTIME>%ld</ENDTIME>",this -> endtime);
	dest += slask;
	dest += "</JOBS>";
	return dest;
}


std::string Jobs::xml(const std::string& tag,const std::string& xvalx)
{
	Query q(*database);
	std::string dest;
	char slask[200];
	dest = "<JOBS " + tag + "=\"" + xvalx + "\">";
	sprintf(slask,"<ID>%ld</ID>",this -> id);
	dest += slask;
	dest += "<JSCRIPT>" + q.GetDatabase().xmlsafestr(this -> jscript) + "</JSCRIPT>";
	dest += "<JOBNAME>" + q.GetDatabase().xmlsafestr(this -> jobname) + "</JOBNAME>";
	dest += "<OUTPUTFILE>" + q.GetDatabase().xmlsafestr(this -> outputfile) + "</OUTPUTFILE>";
	sprintf(slask,"<STATUS>%ld</STATUS>",this -> status);
	dest += slask;
	sprintf(slask,"<STARTTIME>%ld</STARTTIME>",this -> starttime);
	dest += slask;
	sprintf(slask,"<ENDTIME>%ld</ENDTIME>",this -> endtime);
	dest += slask;
	dest += "</JOBS>";
	return dest;
}


size_t Jobs::num_cols()
{
	return 7;
}


void Jobs::clear()
{
	this -> id = 0;
	this -> jscript = "";
	this -> jobname = "";
	this -> outputfile = "";
	this -> status = 0;
	this -> starttime = 0;
	this -> endtime = 0;
}


void Jobs::spawn(const std::string& sql)
{
	Query q(*database);
	std::string temp;

	clear();

	if (!strncasecmp(sql.c_str(),"select * ",9))
	{
		temp = "select id,jscript,jobname,outputFile,status,startTime,endTime " + sql.substr(9);
	} else
		temp = sql;
	q.get_result(temp);
	if (q.fetch_row())
	{
		this -> id = q.getval(0);																				// 0 - id integer
		this -> jscript = q.getstr(1);																				// 1 - jscript varchar(100)
		this -> jobname = q.getstr(2);																				// 2 - jobname varchar(100)
		this -> outputfile = q.getstr(3);																				// 3 - outputfile varchar(256)
		this -> status = q.getval(4);																				// 4 - status integer
		this -> starttime = q.getval(5);																				// 5 - starttime integer
		this -> endtime = q.getval(6);																				// 6 - endtime integer
		new_object = 0;
	} else
		clear();
	q.free_result();
}


void Jobs::spawn(Query *qd,int offset)
{
	clear();

	this -> id = qd -> getval(0 + offset);																				// 0 - id integer
	this -> jscript = qd -> getstr(1 + offset);																				// 1 - jscript varchar(100)
	this -> jobname = qd -> getstr(2 + offset);																				// 2 - jobname varchar(100)
	this -> outputfile = qd -> getstr(3 + offset);																				// 3 - outputfile varchar(256)
	this -> status = qd -> getval(4 + offset);																				// 4 - status integer
	this -> starttime = qd -> getval(5 + offset);																				// 5 - starttime integer
	this -> endtime = qd -> getval(6 + offset);																				// 6 - endtime integer
}


// End of implementation of class 'Jobs'

} // End of namespace
