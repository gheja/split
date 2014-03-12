#include <stdio.h>
#include <sys/stat.h>

int file_exists(const char *filename)
{
    struct stat st;
    int result = stat(filename, &st);
    return result == 0;
}

int main(int argc, char *argv[])
{
	FILE *f_in, *f_out;
	char filename[255];
	unsigned char buffer[1024 * 1024]; // 1 MiB
	unsigned int blocks_per_part = 1024; // 1 MiB * 1024 = 1 GiB
	unsigned long int i, j, bytes_read, bytes_written;
	
	if (argc != 3)
	{
		fprintf(stdout, "split for Windows v0.1 - https://github.com/gheja/split\n");
		fprintf(stdout, "\n");
		fprintf(stdout, "This utility splits files into several parts, that's all.\n");
		fprintf(stdout, "\n");
		fprintf(stdout, "The program will try to open the input file in read-only mode and then split it\n");
		fprintf(stdout, "to multiple %u MiB (%u bytes) parts. Output filenames will be the\n", sizeof(buffer) * blocks_per_part / 1024 / 1024, sizeof(buffer) * blocks_per_part);
		fprintf(stdout, "<output file prefix><part number>, where <part number> is an 8 character long\n");
		fprintf(stdout, "zero padded integer starting from 0, so if the <output file prefix> is \"part\"\n");
		fprintf(stdout, "then the created files will be part00000000, part00000001, part00000002 and so\n");
		fprintf(stdout, "on.\n");
		fprintf(stdout, "\n");
		fprintf(stdout, "The files created are simply the input file split into multiple parts so \n");
		fprintf(stdout, "joining the parts together again is simple as copying more binary files (use\n");
		fprintf(stdout, "/B) with one destination:\n");
		fprintf(stdout, "   copy /b part00000000+part00000001+part00000002 joined.txt\n");
		fprintf(stdout, "\n");
		fprintf(stdout, "Note: the input file should exactly match (checksums must be equal) with the\n");
		fprintf(stdout, "joined one. If this is not the case, something went wrong for sure!\n");
		fprintf(stdout, "\n");
		fprintf(stdout, "The block size is currently hard-coded, might change in the future.\n");
		fprintf(stdout, "\n");
		fprintf(stdout, "Usage:\n");
		fprintf(stdout, "  split <input filename> <output file prefix>\n");
		fprintf(stdout, "\n");
		fprintf(stdout, "Example:\n");
		fprintf(stdout, "  split test.txt part\n");
		fprintf(stdout, "\n");
		fprintf(stdout, "Bugs? Contact? See the project page!\n");
		fprintf(stdout, "  https://github.com/gheja/split\n");
		return 0;
	}

	if (!file_exists(argv[1]))
	{
        fprintf(stderr, "Input file does not exist, exiting.\n");
        exit(1);
	}

	f_in = fopen(argv[1], "rb");
	if (f_in == NULL)
	{
        fprintf(stderr, "Could not open input file for reading, exiting.\n");
		exit(1);
	}
	
	i = 0;
	
	while (!feof(f_in))
	{
		sprintf(filename, "%s%08d", argv[2], i);
		
		if (file_exists(filename))
		{
			fprintf(stderr, "Output file already exists, exiting.\n", filename);
			fclose(f_in);
			exit(1);
		}
		
		f_out = fopen(filename, "wb");

		if (f_out == NULL)
		{
			fprintf(stderr, "Could not open output file for writing, exiting.\n", filename);
			fclose(f_in);
			exit(1);
		}
		
		fprintf(stdout, "%s\n", filename);

	    j = 0;
	    while (!feof(f_in) && j < blocks_per_part)
	    {
			bytes_read = fread(buffer, 1, sizeof(buffer), f_in);
			bytes_written = fwrite(buffer, 1, bytes_read, f_out);
			
			if (bytes_read != bytes_written)
			{
				fprintf(stderr, "Read %u bytes but could only write %u bytes. Disk full? Exiting.\n", bytes_read, bytes_written);
				fclose(f_out);
				fclose(f_in);
				exit(1);
			}
			
			j++;
		}
		fclose(f_out);
		i++;
	}
	fclose(f_in);
	
	return 0;
}
