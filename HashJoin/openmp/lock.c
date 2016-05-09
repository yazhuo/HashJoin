//#include <omp.h>
//#include <stdio.h>
//
////int main()
////{
////	int x = 0;
////
////#pragma omp parallel num_threads(50) shared(x)
////	{
////#pragma omp critical
////		x = x + 1;
////	}
////
////	printf("%d\n", x);
////}
//static omp_lock_t lock;
//
//int main()
//{
//	omp_init_lock(&lock);
//	int i;
//#pragma omp parallel for
//	for (i = 0; i < 5; i++)
//	{
//		omp_set_lock(&lock);
//		printf("%d+\n", omp_get_thread_num());
//		printf("%d-\n", omp_get_thread_num());
//		omp_unset_lock(&lock);
//	}
//	omp_destroy_lock(&lock);
//	return 0;
//}