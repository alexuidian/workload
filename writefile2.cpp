#include<iostream>
#include<fstream>
#include<string.h>
#include<ctime>
#include <unistd.h> 
#include<cstdlib>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

unsigned int random_seed=0;
#if defined(_WIN32)
#include <Windows.h>

#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
#include <unistd.h>	/* POSIX flags */
#include <time.h>	/* clock_gettime(), time() */
#include <sys/time.h>	/* gethrtime(), gettimeofday() */

#if defined(__MACH__) && defined(__APPLE__)
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif

#else
#error "Unable to define getRealTime( ) for an unknown OS."
#endif



double getRealTime( )
{
#if defined(_WIN32)
	FILETIME tm;
	ULONGLONG t;
	#if defined(NTDDI_WIN8) && NTDDI_VERSION >= NTDDI_WIN8
		/* Windows 8, Windows Server 2012 and later. ---------------- */
		GetSystemTimePreciseAsFileTime( &tm );
	#else
		/* Windows 2000 and later. ---------------------------------- */
		GetSystemTimeAsFileTime( &tm );
	#endif
	t = ((ULONGLONG)tm.dwHighDateTime << 32) | (ULONGLONG)tm.dwLowDateTime;
	return (double)t / 10000000.0;

#elif (defined(__hpux) || defined(hpux)) || ((defined(__sun__) || defined(__sun) || defined(sun)) && (defined(__SVR4) || defined(__svr4__)))
	/* HP-UX, Solaris. ------------------------------------------ */
	return (double)gethrtime( ) / 1000000000.0;

#elif defined(__MACH__) && defined(__APPLE__)
	/* OSX. ----------------------------------------------------- */
	static double timeConvert = 0.0;
	if ( timeConvert == 0.0 )
	{
		mach_timebase_info_data_t timeBase;
		(void)mach_timebase_info( &timeBase );
		timeConvert = (double)timeBase.numer /
			(double)timeBase.denom /
			1000000000.0;
	}
	return (double)mach_absolute_time( ) * timeConvert;

#elif defined(_POSIX_VERSION)
	/* POSIX. --------------------------------------------------- */
	#if defined(_POSIX_TIMERS) && (_POSIX_TIMERS > 0)
		{
			struct timespec ts;
			#if defined(CLOCK_MONOTONIC_PRECISE)
				/* BSD. --------------------------------------------- */
				const clockid_t id = CLOCK_MONOTONIC_PRECISE;
			#elif defined(CLOCK_MONOTONIC_RAW)
				/* Linux. ------------------------------------------- */
				const clockid_t id = CLOCK_MONOTONIC_RAW;
			#elif defined(CLOCK_HIGHRES)
				/* Solaris. ----------------------------------------- */
				const clockid_t id = CLOCK_HIGHRES;
			#elif defined(CLOCK_MONOTONIC)
				/* AIX, BSD, Linux, POSIX, Solaris. ----------------- */
				const clockid_t id = CLOCK_MONOTONIC;
			#elif defined(CLOCK_REALTIME)
				/* AIX, BSD, HP-UX, Linux, POSIX. ------------------- */
				const clockid_t id = CLOCK_REALTIME;
			#else
				const clockid_t id = (clockid_t)-1;	/* Unknown. */
			#endif /* CLOCK_* */
			
			if ( id != (clockid_t)-1 && clock_gettime( id, &ts ) != -1 )
			return (double)ts.tv_sec +
				(double)ts.tv_nsec / 1000000000.0;
		/* Fall thru. */
	}
#endif /* _POSIX_TIMERS */

	/* AIX, BSD, Cygwin, HP-UX, Linux, OSX, POSIX, Solaris. ----- */
	struct timeval tm;
	gettimeofday( &tm, NULL );
	return (double)tm.tv_sec + (double)tm.tv_usec / 1000000.0;
#else
	return -1.0;		/* Failed. */
#endif
}



using namespace std;

void fillbuffer(char *buffer, unsigned long long ts)
{
	time_t t;
unsigned long long i;
//srand(random_seed);
	for(i=0;i<ts-1;i++)
	{
		int r=rand()%rand();
	
		buffer[i]=(r)%26+'a';
	}
	buffer[i]='\0';

}

int main(int argc, char *argv[])
{

if(argc < 2) 
{

	cout <<  
	"Usage: " << argv[0] << " Filename filesize_in_GB  r/w/b/n/o  [buffersize] runtime  randomseed\n " <<
	"r: random_read\n w: random_write \n n: pure_random_read \n o: directio_random_read\n runtime is in seconds \n buffer size is in KB \n Filesize in GB \n all parametrs are compulsory and parameters which are not going to be used can be passwd with any value \n"
	;
	return -1;

}

if(!strcmp(argv[1],"--help"))
{
	cout <<  
	"Usage: " << argv[0] << " Filename filesize_in_GB  r/w/b/n/o  [buffersize] runtime randomseed \n " <<
	"r: random_read\n w: random_write \n n: pure_random_read \n o: directio_random_read\n m: modify \n runtime is in seconds \n buffer size is in KB \n Filesize in GB \n all parametrs are compulsory and parameters which are not going to be used can be passwd with any value \n"
	;
	return -2;
}

	if(argc < 7)
	{
	cout << "Usage: " << argv[0] << " Filename filesize_in_GB  r/w/b/n/o/m  [buffersize] runtime  randomseed\n " << argv[0] << "--help " << endl;
	return -1;
	}

//cout << argv[0] << argv[1]  << argv[2] << argv[3] ;

int timesec;

char filename[20];
strcpy(filename,argv[1]);

cout << argv[1] ;


double totalsize=atof(argv[2]);

timesec=atoi(argv[5]);

cout << timesec << " seconds\n";

int mode=0;  // READ=0, WRITE=1, R+W=2, RR=3

time_t t;

switch(argv[3][0])
{
case 'r':
	mode=0;
	break;
case 'w':
	mode=1;	
	break;
case 'b':
	mode=2;
	break;
case 'n':
	mode=3;
	break;
case 'o':
	mode=4;
	break;
case 'm':
	mode=5;
	break;
default:
	cout << "Wrong mode given \n exiting \n";
	return 0;
	break;
}


int buffsize;
if(argc>=4)
buffsize=atoi(argv[4]);
else buffsize=4;

random_seed=atoi(argv[6]);
srand(random_seed);


unsigned long long tb=buffsize*1000;
char *buffer=(char *) malloc(tb);
fillbuffer(buffer, tb);

	double seq_perf2=0,rand_perf2=0,xperf2=0,actualtime2=0;
	unsigned long long int tcc2=0, to2=0;
		double seq_perf1=0,rand_perf1=0,xperf1=0,actualtime1=0;
	unsigned long long int tcc1=0, to1=0;



//cout << "YALA Bqalle Balle" << endl;

if((mode == 1) || (mode == 2))
{	
	cout << "#Write mod\n";
	time_t starttime=clock();
	time_t end_time=clock();
	ofstream myfile (filename, ios::out | ios::binary);
	double startt=getRealTime();
	double endt=getRealTime();
	unsigned long long int total_cpu_cycles=0, total_operations=0;

	if(myfile.is_open())
	{
		fillbuffer(buffer,tb);
		unsigned long int totals=totalsize*1000*1000;
		unsigned long long counts=totals/buffsize;
		int rem= totals% buffsize;
		//cout << totals << " KB  " << buffsize << " KB  "  << counts << endl ;
		
		time_t starttime=clock();
		while(counts--)
		{
			fillbuffer(buffer,tb);
			starttime=clock();
			myfile.write(buffer,tb);
			end_time=clock();
			total_cpu_cycles+=end_time - starttime ;
			total_operations++;
				
		//	cout << counts << endl;
		}
		if(rem)
		{
			starttime=clock();
			myfile.write(buffer,rem);
			end_time=clock();
			total_cpu_cycles+=end_time - starttime ;
			total_operations++;
		}
	//cout << "First part done \n";
		
	 double random_start = getRealTime();	
	 
        //srand(random_seed);
		

	int random_ops=0;	
	 while(timesec > (getRealTime() - random_start) )
	  {
		
		unsigned long long MOD= totals - buffsize;
		if(MOD < 1) break;
		
		time_t t;
		unsigned long long int my_big_random = 11 * rand() ^ (rand() << 15) ^ ((long long int) rand() << 30);		
		unsigned long long int randomint= (my_big_random % MOD );
		
		starttime=clock();
		fillbuffer(buffer,tb);
		myfile.seekp(randomint*1000,ios::beg);
		myfile.write (buffer, tb);
		end_time=clock();
		total_cpu_cycles+=end_time - starttime ;
		total_operations++;
		random_ops++;
	        //cout << buffer << endl; 	
		//fillbuffer(buffer, tb);
		  
		//cout << myfile.tellg() << "Location  " << endl  ;
	 }
	




		myfile.close();
		endt=getRealTime();

		double seq_performance=(double)(totals);
		double ran_performance=(double)(random_ops*buffsize);
		double xperformance=(double) (total_operations*buffsize);
		//cout << " Actual time is  " << (endt-startt) << endl ;
		seq_performance= seq_performance / ((random_start - startt )*1000);
		ran_performance= ran_performance/ ((endt - random_start)*1000);
		xperformance= xperformance /((endt - startt)*1000);
		//cout << "CPU time is " << total_cpu_cycles << "  and total operations are " << total_operations << endl;
		//cout << "Performance for writing for file of  " <<  totals << " MB" << " is " << performance << " MBPS" << endl ;
		if(mode !=2)
		{
			
			cout << "#seq_perf rand_perf #avf_perf #total_ops #cpucycles/ops #cpucycles\n";
			cout << seq_performance << " " << ran_performance << " " << xperformance << " " << total_operations << " " << total_cpu_cycles/total_operations <<" " << total_cpu_cycles <<" "  << total_operations-random_ops << " " << random_ops << " "<< total_operations << endl;
			//cout << "avg_performance: " << performance << " KBPS"<< endl;
			//cout << "xavg_performance: " << xperformance << " KBPS" << endl;
			//cout << "cpu_cycles:" << total_cpu_cycles << endl;	
			//cout << "total_operation:" << total_operations << endl;
		}
		seq_perf1=seq_performance;
		rand_perf1=ran_performance;
		xperf1=xperformance;
		actualtime1=(endt-startt);
		tcc1=total_cpu_cycles;
		to1=total_operations;
	}
	else cout << "Unable to open file\n";	
}

if((mode==0) || (mode ==2))
{
	cout << "Read Mode \n";
	ifstream myfile;	
 	myfile.open(filename, ios::in|ios::binary|ios::ate);
	time_t starttime=clock();
    time_t end_time=clock();
    double startt=getRealTime();
	double endt=getRealTime();
	unsigned long long int total_cpu_cycles=0, total_operations=0;

	 if(myfile.is_open())
	{
	unsigned long long size= myfile.tellg();
	cout<< "filesize: " << size  << endl ;
	unsigned long long size_in_mb = size / 1000;
	
	unsigned long long counts = size_in_mb/buffsize;
	
	int rem = size_in_mb %  buffsize;
		
	myfile.seekg(0,ios::beg);
	//cout << myfile.tellg() << "Location  " << endl  ;
	//cout << counts  << "  " << rem << endl;
	while(counts--)
	{
		//fillbuffer(buffer, tb);
		starttime=clock();
		myfile.read (buffer, tb); 	
		end_time=clock();
		total_cpu_cycles+=end_time - starttime ;
		total_operations++;
				
		//cout << myfile.tellg() << "Location  " << endl  ;
	}
	if(rem)
	{
			starttime=clock();
			myfile.read(buffer,rem);
			end_time=clock();
			total_cpu_cycles+=end_time - starttime ;
			total_operations++;
	}

	double random_start=getRealTime();

        //srand(random_seed);
	int random_ops=0;
	 while(timesec > (getRealTime() - random_start) )
	{
		
		unsigned long long MOD= size_in_mb - buffsize;
		if(MOD < 1) break;
		
		unsigned long long int my_big_random = 11 * rand() ^ (rand() << 15) ^ ((long long int) rand() << 30);
		unsigned long long int randomint= (my_big_random % MOD );
		
		starttime=clock();
		myfile.seekg(randomint*1000,ios::beg);
		myfile.read (buffer, tb);
		end_time=clock();
		total_cpu_cycles+=end_time - starttime ;
		total_operations++;
		random_ops++;
	    //cout << buffer << endl; 	
		//fillbuffer(buffer, tb);
		if(timesec <= (getRealTime() - random_start) ) break;		
		//cout << myfile.tellg() << "Location  " << endl  ;
	}


	
		myfile.close();
		endt=getRealTime();

		double seq_performance=(double)(size_in_mb);
		double ran_performance=(double)(random_ops*buffsize);
		double xperformance=(double) (total_operations*buffsize);
		//cout << " Actual time is  " << (endt-startt) << endl ;
		seq_performance= seq_performance / ((random_start - startt )*1000);
		ran_performance= ran_performance/ ((endt - random_start)*1000);
		xperformance= xperformance /((endt - startt)*1000);
		//cout << "CPU time is " << total_cpu_cycles << "  and total operations are " << total_operations << endl;
		//cout << "Performance for writing for file of  " <<  totals << " MB" << " is " << performance << " MBPS" << endl ;
		if(mode !=2)
		{
			
			cout << "#seq_perf rand_perf #avf_perf #total_ops #cpucycles/ops #cpucycles\n";
			cout << seq_performance << " " << ran_performance << " " << xperformance << " " << total_operations << " " << total_cpu_cycles/total_operations << " " << total_cpu_cycles << " " << total_operations-random_ops << " "<< random_ops<< " " << total_operations << endl;
			//cout << "avg_performance: " << performance << " KBPS"<< endl;
			//cout << "xavg_performance: " << xperformance << " KBPS" << endl;
			//cout << "cpu_cycles:" << total_cpu_cycles << endl;	
			//cout << "total_operation:" << total_operations << endl;
		}
		seq_perf2=seq_performance;
		rand_perf2=ran_performance;
		xperf2=xperformance;
		actualtime2=(endt-startt);
		tcc2=total_cpu_cycles;
		to2=total_operations;
	}
	else cout << "Unable to open file";	

	cout << "#Done\n";

}

if(mode ==2 )
{

cout << "# final: seq_perf rand_perf #avf_perf #total_ops #cpucycles/ops #cpucycles \n";
cout << (seq_perf1+seq_perf2) /2 << " " << (rand_perf2+rand_perf1)/2 << " " << (xperf1 + xperf2)/2 << " " << tcc1+tcc2 << " " << (tcc1+tcc2)/(to1+to2) << endl;

//cout << "avg_performance: " << (perf1+perf2)/2 << " KBPS"<<endl;
//cout << "xavg_performance: " << (xperf1+xperf2)/2 << " KBPS"<<endl;
//cout << "cpu_cycles:" << tcc1+tcc2 << endl;
//cout << "total_operation:" << to1+ to2 << endl;

}


if(mode==4)
{

	cout << "#DirectIO mode\n" << endl;
	struct stat fi;
	stat(filename, &fi);
	int blocksize=(int) fi.st_blksize;
	unsigned long long int filesize_in_B= (unsigned long long int) fi.st_size;
	cout << "#File size: "<< filesize_in_B << endl << "Block size:" << blocksize << endl;
	int f=open(filename, O_RDWR | O_DIRECT );
	char *bufferx=(char *)malloc(blocksize);
	if(f != -1)
	{
		posix_memalign((void **)&bufferx, blocksize, blocksize);

	time_t starttime=clock();
    time_t end_time=clock();
    double startt=getRealTime();
	double endt=getRealTime();
	unsigned long long int total_cpu_cycles=0, total_operations=0;

	//char *block=(char *)buffer;
	//bufferx=(void *)malloc(blocksize);
	//fillbuffer(block, blocksize);
	unsigned long long int counts=filesize_in_B/blocksize;
	int rem=filesize_in_B%blocksize;
	
	while(counts--)
	{
		starttime=clock();
		read(f, bufferx, blocksize);	
		end_time=clock();
		total_cpu_cycles+=end_time - starttime ;
		total_operations++;
	//	cout << bufferx;
		//fillbuffer(buffer, tb);		
		//cout << myfile.tellg() << "Location  " << endl  ;
	}
	if(rem)
	{
			starttime=clock();
			read(f, bufferx, blocksize);
			end_time=clock();
			total_cpu_cycles+=end_time - starttime ;
			total_operations++;
	}

	double random_start=getRealTime();

        //srand(random_seed);
	
	unsigned long long int random_ops=0;
	
	while(timesec > (getRealTime() - random_start) ) 		
	{
		
		unsigned long long MOD= filesize_in_B - blocksize;
		if(MOD < 1) break;

	//	time_t t;

		unsigned long long int my_big_random = 11 * rand() ^ (rand() << 15) ^ ((long long int) rand() << 30);
		unsigned long long int randomint= (my_big_random % MOD );
	
//		unsigned long long int randomint= (rand() % MOD );
	
		lseek(f, randomint, SEEK_SET);
		
		starttime=clock();
		read(f, bufferx, blocksize);	
		end_time=clock();
		total_cpu_cycles+=end_time - starttime ;
		total_operations++;
		random_ops++;
	    //cout << buffer << endl; 	
		//fillbuffer(buffer, tb);
	//	if(timesec <= (getRealTime() - random_start) ) break;		
		//cout << myfile.tellg() << "Location  " << endl  ;
	}
	



	close(f);
	endt=getRealTime();

	int filesize_inMB=(filesize_in_B/1000);
	
	
		double seq_performance=(double)(filesize_inMB);
                double ran_performance=(double)(random_ops*blocksize);
                double xperformance=(double) (total_operations*blocksize);
		cout << "total ops: " << total_operations << endl;
		cout << "total time: " << endt - startt << endl;
                seq_performance= seq_performance / ((random_start - startt )*1000);
                ran_performance= ran_performance/ ((endt - random_start)*1000);
                xperformance= xperformance /((endt - startt)*1000);
         cout << "#seq_perf rand_perf #avf_perf #total_ops #cpucycles/ops #cpucycles\n";
       	 cout << seq_performance << " " << ran_performance << " " << xperformance << " " << total_operations << " " << total_cpu_cycles/total_operations << endl;

	/*
	double performance = (double) (filesize_inMB);
	double xperformance=(double) (total_operations*blocksize);
	//cout << " Actual time is  " << (endt-startt) << endl ;
		
	performance= performance / (endt - startt );
	xperformance= xperformance / (endt - startt);

	//cout << "CPU time is " << total_cpu_cycles << "  and total operations are " << total_operations << endl;
	//cout << "Performance for reading for file of  " <<  size_in_mb << " MB" << " is " << performance << " MBPS" << endl ;
	
	cout << "avg_performance: " << performance << " KBPS"<< endl;
	cout << "xavg_performance: " << xperformance << " KBPS"<< endl;
	cout << "cpu_cycles:" << total_cpu_cycles << endl;	
	cout << "total_operation:" << total_operations << endl;
	*/
	}
	else 
		cout<< "Error opening file \n";
	free(bufferx);
	

}


if(mode == 3)
{

	cout << "#Random Read \n";
	ifstream myfile;	
 	myfile.open(filename, ios::in|ios::binary|ios::ate);
	time_t starttime=clock();
    	time_t end_time=clock();
    	double startt=getRealTime();
	double endt=getRealTime();
	unsigned long long int total_cpu_cycles=0, total_operations=0;
	 if(myfile.is_open())
	{
	unsigned long long size= myfile.tellg();
	cout<< "#File size: " << size  << endl ;
	unsigned long long size_in_mb = size / 1000;
	
	unsigned long long counts = size_in_mb/buffsize;
	
	int rem = size_in_mb %  buffsize;
		
	myfile.seekg(0,ios::beg);
	//cout << myfile.tellg() << "Location  " << endl  ;
	//cout << counts  << "  " << rem << endl;
	time_t start=clock();
	time_t end;
        //srand(random_seed);
	float random_start=getRealTime();

	while(timesec > (getRealTime() - random_start) )
	{
		
		unsigned long long MOD= size - tb;
		if(MOD < 1) break;

		time_t t;

		unsigned long long int my_big_random =11 * rand() ^ (rand() << 15) ^ ((long long int) rand() << 30);
		unsigned long long int randomint= (my_big_random % MOD );
		//unsigned long long int randomint= (rand() % MOD );
		
		starttime=clock();
		myfile.seekg(randomint,ios::beg);
		myfile.read (buffer, tb);
		end_time=clock();
		total_cpu_cycles+=end_time - starttime ;
		total_operations++;
	        //cout << buffer << endl; 	
		//fillbuffer(buffer, tb);
		if(timesec <= (getRealTime() - startt) ) break;		
		//cout << myfile.tellg() << "Location  " << endl  ;
	}
	if(rem)
	{
			starttime=clock();
			myfile.read(buffer,rem);
			end_time=clock();
			total_cpu_cycles+=end_time - starttime ;
			total_operations++;
	}


	myfile.close();	

  	endt=getRealTime();
	

	double performance = (double) (size_in_mb);
	double xperformance = (double) (total_operations*buffsize);
	//cout << " Actual time is  " << (endt-startt) << endl ;
	performance= performance / (endt - startt );
	xperformance= xperformance / (endt - startt );
	//cout << "CPU time is " << total_cpu_cycles << "  and total operations are " << total_operations << endl;
	//cout << "Performance for reading for file of  " <<  size_in_mb << " MB" << " is " << performance << " MBPS" << endl ;
	//if(mode !=2)
	//{
		cout << "avg_performance: " << performance << " KBPS"<<endl;
		cout << "xavg_performance: " << xperformance << " KBPS"<<endl;
		cout << "cpu_cycles:" << total_cpu_cycles << endl;	
		cout << "total_operation:" << total_operations << endl;
	//}

	}
	else cout << "Could not open file \n";
	cout << "#Done\n";

}

if(mode==5)
{

	cout << "#Write mod\n";
        fstream myfile(filename, ios::binary | ios::out | ios::in);
	
        if(myfile.is_open())
        {
		tb=1000;
                fillbuffer(buffer,tb);
                unsigned long int totals=totalsize*1000*1000;
                unsigned long long counts=totals/buffsize;
		counts=counts/2;
		
		printf("Counts: %d",(int) counts);
                
		while(counts--)
                {
			unsigned long long MOD= totals - buffsize;
			if(MOD < 1) { printf("ERROR: filesize very less\n"); break;}
			unsigned long long int my_big_random = 11 * rand() ^ (rand() << 15) ^ ((long long int) rand() << 30);
                	unsigned long long int randomint= (my_big_random % MOD );			
			//printf("Random ints are%llu \n", randomint);
                        fillbuffer(buffer,tb);
			myfile.seekp(randomint*1000,ios::beg);
                        myfile.write(buffer,tb);

                }
	}
	else {
		printf("ERROR: file coutld not open\n");
	}          	


}



free(buffer);
return 0;
}
