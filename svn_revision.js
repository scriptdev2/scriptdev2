var FSO = WScript.CreateObject("Scripting.FileSystemObject");
var OutputFile = FSO.CreateTextFile("svn_revision.h", true);
var EntriesFile = FSO.OpenTextFile(".svn\\entries", 1, false);

EntriesFile.SkipLine();
EntriesFile.SkipLine();
EntriesFile.SkipLine();

var revision, date_time;

revision = Number(EntriesFile.ReadLine());

EntriesFile.SkipLine();
EntriesFile.SkipLine();
EntriesFile.SkipLine();
EntriesFile.SkipLine();
EntriesFile.SkipLine();

date_time = EntriesFile.ReadLine();

OutputFile.WriteLine("#ifndef __SVN_REVISION_H__");
OutputFile.WriteLine("#define __SVN_REVISION_H__");
OutputFile.WriteLine(" #define SVN_REVISION \"" + revision + "\"");
OutputFile.WriteLine(" #define SVN_DATE \"" + date_time.substr(0, 10) + "\"");
OutputFile.WriteLine(" #define SVN_TIME \"" + date_time.substr(11, 8) + "\"");
OutputFile.WriteLine("#endif // __SVN_REVISION_H__");

OutputFile.Close();
EntriesFile.Close();

