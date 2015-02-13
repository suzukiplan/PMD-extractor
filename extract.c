/* PMD extractor */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc,char* argv[])
{
	char fn[16];
	int ret=-1;
	FILE* fp=NULL;
	char* rbuf=NULL;
	size_t rsize;
	int i;
	int j;
	int n;

	if(argc<1) {
		puts("usage: extpmd file");
		goto ENDPROC;
	}

	fp=fopen(argv[1],"rb");
	if(NULL==fp) {
		puts("Could not open the input file.");
		goto ENDPROC;
	}

	fseek(fp,0,SEEK_END);
	rsize=(size_t)ftell(fp);
	fseek(fp,0,SEEK_SET);
	if(rsize<1) goto ENDPROC;

	rbuf=(char*)malloc(rsize);
	if(NULL==rbuf) {
		puts("No memory.");
		goto ENDPROC;
	}
	fread(rbuf,rsize,1,fp);
	fclose(fp);
	fp=NULL;
	printf("size=%u\n",(unsigned int)rsize);

	n=0;
	j=-1;
	for(i=0;i<rsize;i++) {
		if(rbuf[i  ]==0x00
		&& rbuf[i+1]==0x1A
		&& rbuf[i+2]==0x00) {
			if(j!=-1) {
				sprintf(fn,"song%04d.m",n++);
				puts(fn);
				fp=fopen(fn,"wb");
				fwrite(&rbuf[j],i-j,1,fp);
				fclose(fp);
				fp=NULL;
			}
			j=i;
		}
	}

	if(j!=-1) {
		sprintf(fn,"song%04d.m",n++);
		puts(fn);
		fp=fopen(fn,"wb");
		fwrite(&rbuf[j],i-j,1,fp);
		fclose(fp);
		fp=NULL;
	}

	ret=0;

ENDPROC:
	if(fp) fclose(fp);
	if(rbuf) free(rbuf);
	return ret;
}
