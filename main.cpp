#include <iostream>
#include <chrono>
#include <random>
#include <cfloat>
#include <immintrin.h>

extern "C" {double fpuSQRT(double num)__attribute__((fastcall)); }
extern "C" {double sqSQRT(double num)__attribute__((fastcall)); }

double averageCPUDelay;

std::chrono::duration<double> benchFPU(double num = rand()) {

	auto sTime = std::chrono::high_resolution_clock::now();
	fpuSQRT(num);
	auto eTime = std::chrono::high_resolution_clock::now();
	
	return eTime - sTime;
}

std::chrono::duration<double> benchCMATH(double num) {
	
	auto sTime = std::chrono::high_resolution_clock::now();
	sqrt(num);
	auto eTime = std::chrono::high_resolution_clock::now();

	return eTime - sTime;
}

std::chrono::duration<double> benchSQ(double num) {
	
	auto sTime = std::chrono::high_resolution_clock::now();
	sqSQRT(num);
	auto eTime = std::chrono::high_resolution_clock::now();

	return eTime - sTime;
}

std::chrono::duration<double> benchSQINTRINSIC(double num) {

	double out = 0;
	auto sTime = std::chrono::high_resolution_clock::now();

	__m128d outval = _mm_sqrt_pd(_mm_loadu_pd(&num));

	auto eTime = std::chrono::high_resolution_clock::now();

	return eTime - sTime; 
}

std::chrono::duration<double> benchSQINLINE(double num) {

	auto sTime = std::chrono::high_resolution_clock::now();

	asm("sqrtsd %0,%%XMM0" ::"x"(num));

	auto eTime = std::chrono::high_resolution_clock::now();

	return eTime - sTime; 
}

double lfpuTime,lcmathTime,lsqTime,lsqIntrisicTime,lsqinlineTime;
void bench(unsigned long iter) {
	std::mt19937 generator(time(nullptr));
	std::uniform_real_distribution<double> dis(0.0, DBL_MAX);
	std::chrono::duration<double> fpuTime = std::chrono::duration<double>::zero();
	std::chrono::duration<double> cmathTime = std::chrono::duration<double>::zero();
	std::chrono::duration<double> sqTime = std::chrono::duration<double>::zero();
	std::chrono::duration<double> sqINTRINSICTIME = std::chrono::duration<double>::zero();
	std::chrono::duration<double> sqInlineTime = std::chrono::duration<double>::zero();
	for (unsigned long i = 0; i < iter; i++) {
		double num = dis(generator);
		 fpuTime += benchFPU(num);
		 cmathTime += benchCMATH(num);
		 sqTime += benchSQ(num);
		 sqINTRINSICTIME += benchSQINTRINSIC(num);
		 sqInlineTime += benchSQINLINE(num);
	}
	lfpuTime += fpuTime.count();
	lcmathTime += cmathTime.count();
	lsqTime += sqTime.count();
	lsqIntrisicTime += sqINTRINSICTIME.count();
	lsqinlineTime += sqInlineTime.count();
	std::cout << iter << " iterations finished" << std::endl << std::fixed
		<< "FPU time: " << fpuTime.count() - averageCPUDelay << std::endl
		<< "CMATH time: " << cmathTime.count() - averageCPUDelay << std::endl
		<< "SQ Time: " << sqTime.count() - averageCPUDelay << std::endl
		<< "SQ-INTRINSIC time: " << sqINTRINSICTIME.count() - averageCPUDelay << std::endl 
		<< "SQ-Inline time: " << sqInlineTime.count() - averageCPUDelay << std::endl;
}

#define DEFAULT_ITERATION 100000000

int main(int argc, char* argv[]) {
    unsigned long bench_iter;
    
    if(argc == 2)
        bench_iter = atoi(argv[1]);
    else
        bench_iter = DEFAULT_ITERATION;

	std::cout << std::fixed << "FPU benchmark" << std::endl;
	unsigned long iter = 2;

    std::chrono::duration<double> baseDelay = std::chrono::duration<double>::zero();

    for(int i = 0; i < 200; i++){
        auto sTime = std::chrono::high_resolution_clock::now();
	
	    auto eTime = std::chrono::high_resolution_clock::now();
	
	    baseDelay += eTime - sTime;
    }

    averageCPUDelay = baseDelay.count() / 200;
    averageCPUDelay /= 200;
    
    std::cout << std::fixed << "Base CPU delay is " << averageCPUDelay << std::endl;


	for(int i = 0; i < 10; i++)
 	    bench(bench_iter);
	
    std::cout << "TOTAL AVERAGE OF 100 RUNS" << std::endl
	<< "FPU: " << lfpuTime / 10 << std::endl
	<< "CMATH: " << lcmathTime / 10 << std::endl
	<< "SQ: " << lsqTime / 10 << std::endl
	<< "SQ INTRINSIC " << lsqIntrisicTime  / 10 << std::endl
	<< "SQ INLINE" << lsqinlineTime / 10 << std::endl;
}
