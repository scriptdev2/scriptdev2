/* 
 * Copyright (C) 2005-2008 MaNGOS <http://www.mangosproject.org/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <fstream>
#include <sstream>
#include <cstring>

#pragma warning(disable:4996)

int main(int argc, char **argv)
{
    FILE* EntriesFile = fopen(".svn/entries", "r");
    if(!EntriesFile)
        return 1;

    char buf[200];
    int revision;
    char date_str[200];
    char time_str[200];

    fgets(buf,200,EntriesFile);
    if (strstr(buf,"xml") > 0)
    { // svn 1.3.x
      fgets(buf,200,EntriesFile);
      fgets(buf,200,EntriesFile);
      fgets(buf,200,EntriesFile);
      fgets(buf,200,EntriesFile);
      fgets(buf,200,EntriesFile);
      fscanf(EntriesFile,"   committed-date=\"%10sT%8s",date_str,time_str);
      fgets(buf,200,EntriesFile);
      fgets(buf,200,EntriesFile);
      fgets(buf,200,EntriesFile);
      fgets(buf,200,EntriesFile);
      fgets(buf,200,EntriesFile);
      fgets(buf,200,EntriesFile);
      fscanf(EntriesFile,"   revision=\"%i",&revision);
    } else
    { // svn 1.4.x
      fgets(buf,200,EntriesFile);
      fgets(buf,200,EntriesFile);
      fscanf(EntriesFile,"%i",&revision);
      fgets(buf,200,EntriesFile);
      fgets(buf,200,EntriesFile);
      fgets(buf,200,EntriesFile);
      fgets(buf,200,EntriesFile);
      fgets(buf,200,EntriesFile);
      fscanf(EntriesFile,"%10sT%8s",date_str,time_str);
    }

    std::ostringstream newData;

    newData << "#ifndef __SVN_REVISION_H__" << std::endl;
    newData << "#define __SVN_REVISION_H__"  << std::endl;
    newData << " #define SVN_REVISION \"" << revision << "\"" << std::endl;
    newData << " #define SVN_DATE \"" << date_str << "\"" << std::endl;
    newData << " #define SVN_TIME \"" << time_str << "\""<< std::endl;
    newData << "#endif // __SVN_REVISION_H__" << std::endl;

    fclose(EntriesFile);

    std::string oldData;

    if(FILE* HeaderFile = fopen("svn_revision.h","rb"))
    {
        while(!feof(HeaderFile))
        {
            int c = fgetc(HeaderFile);
            if(c < 0)
                break;
            oldData += (char)c;
        }

        fclose(HeaderFile);
    }

    if(newData.str() != oldData)
    {
        if(FILE* OutputFile = fopen("svn_revision.h","wb"))
        {
            fprintf(OutputFile,"%s",newData.str().c_str());
            fclose(OutputFile);
        }
    }

    return 0;
}
