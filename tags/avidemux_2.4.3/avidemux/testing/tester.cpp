#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

static char wgetPath[255];
static char md5sumPath[255];
static char avidemuxPath[255];
static char outputPath[255];
static char outputFile[255];

bool wipeOutputDirectory(void)
{
	DIR *dir = opendir(outputPath);

	if (dir)
	{
		struct dirent *ent;
		char path[255];
		bool error = false;

		getcwd(path, 255);
		chdir(outputPath);

		while ((ent = readdir(dir)) != NULL)
		{
			if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
			{
				if (remove(ent->d_name) != 0)
				{
					fprintf(stderr, "Error removing file (%s) from output directory\n", ent->d_name);
					error = true;
					break;
				}
			}
		}

		chdir(path);
		closedir(dir);

		return !error;
	}
	else
#ifdef __WIN32
		return mkdir(outputPath) == 0;
#else
		return mkdir(outputPath, ACCESSPERMS) == 0;
#endif
}

bool removeIdxFile(const char* inputDirectory, const char* inputFilename)
{
	char curDir[255];
	char idxPath[255 * 2];
	bool success = false;

	getcwd(curDir, 255);

	strcpy(idxPath, inputFilename);
	strcat(idxPath, ".idx");

	chdir(inputDirectory);

	FILE* fp = fopen(idxPath, "rb");

	if (fp == NULL)
		success = true;
	else
	{
		fclose(fp);

		success = (remove(idxPath) == 0);

		if (!success)
			fprintf(stderr, "Unable to delete idx file %s/%s\n", inputDirectory, idxPath);
	}

	chdir(curDir);

	return success;
}

bool performTest(const char* inputDirectory, const char* inputFilename, const char* url, const char* avidemuxParams, const char* md5sum)
{
	char cmdline[2000];

	strcpy(cmdline, wgetPath);
	strcat(cmdline, " --directory-prefix ");
	strcat(cmdline, inputDirectory);
	strcat(cmdline, " --timestamping ");
	strcat(cmdline, url);

	if (system(cmdline) != 0)
	{
		fprintf(stderr, "Error executing %s\n", cmdline);
		return false;
	}

	if (!removeIdxFile(inputDirectory, inputFilename))
		return false;

	strcpy(cmdline, avidemuxPath);
	strcat(cmdline, " ");
	strcat(cmdline, avidemuxParams);

	if (system(cmdline) != 0)
	{
		fprintf(stderr, "Error executing %s\n", avidemuxPath);
		return false;
	}

	if (!removeIdxFile(inputDirectory, inputFilename))
		return false;

	char path[255 * 2];

	strcpy(path, md5sumPath);
	strcat(path, " -b ");
	strcat(path, outputPath);
	strcat(path, "/");
	strcat(path, outputFile);

	FILE *outputStream = popen(path, "r");
	char result[35];

	if (outputStream)
	{
		memset(result, 0, sizeof(result));
		fread(result, 1, sizeof(result) - 1, outputStream);
		fclose(outputStream);

		if (strcmp(result + strlen(result) - 2, " *") == 0)
			result[strlen(result) - 2] = 0;
		else
		{
			fprintf(stderr, "Error parsing MD5 sum: %s\n", result);
			return false;
		}
	}
	else
	{
		fprintf(stderr, "Error executing %s\n", path);
		return false;
	}

	printf("original md5 sum: %s\n", md5sum);
	printf("new md5 sum     : %s\n", result);

	if (strcmp(md5sum, result) != 0)
	{
		fprintf(stderr, "MD5 sums do not match\n");
		return false;
	}

	return true;
}

int main(int argc, char *argv[])
{
	bool success = false;
	FILE* configFile = fopen("tester.cfg", "rt");

	while (true)
	{
		if (argc != 6)
		{
			fprintf(stderr, "Incorrect number of arguments (%d instead of 5)\n", argc);
			break;
		}

		if (!configFile)
		{
			char path[255];

			getcwd(path, 255);
			fprintf(stderr, "Error opening %s/tester.cfg\n", path);

			break;
		}

		if (fgets(wgetPath, 255, configFile))
			wgetPath[strlen(wgetPath) - 1] = 0;
		else
			break;

		if (fgets(md5sumPath, 255, configFile))
			md5sumPath[strlen(md5sumPath) - 1] = 0;
		else
			break;

		if (fgets(avidemuxPath, 255, configFile))
			avidemuxPath[strlen(avidemuxPath) - 1] = 0;
		else
			break;

		if (fgets(outputPath, 255, configFile))
			outputPath[strlen(outputPath) - 1] = 0;
		else
			break;

		if (fgets(outputFile, 255, configFile))
			outputFile[strlen(outputFile) - 1] = 0;
		else
			break;

		if (!wipeOutputDirectory())
			break;

		success = performTest(argv[1], argv[2], argv[3], argv[4], argv[5]);

		break;
	}

	if (configFile)
		fclose(configFile);

	return !success;
}
