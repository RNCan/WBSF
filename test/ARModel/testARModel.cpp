// Except for any way in which it interferes with Cedrick Collomb's 2009
// copyright assertion in the article "Burg’s Method, Algorithm and Recursion":
//
// Copyright (C) 2012, 2013 Rhys Ulerich
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.
//

/** @file
 * Example program using \ref ar::burg_method and \ref ar::evaluate_models.
 */

#include <cmath>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <vector>
#include <random>
#include <chrono>

#include "External/ARModel/ARModel.h"
#include <UnitTest++/UnitTest++.h>

TEST(Sanity)
{
   CHECK_EQUAL(1, 1);
}

int main(int, const char *[])
{
   return UnitTest::RunAllTests();
}

using namespace std;
using namespace WBSF;

//static double rand01()
//{
//    const double x = (double) rand() / RAND_MAX;
//    const double y = (double) rand() / RAND_MAX;
//    const double z = sqrt(-2 * log(x)) * cos(2 * M_PI * y);
//    return z;
//}

// Scale to obtain variance 0.1.
//static double rand0point1()
//{
//    return sqrt(52) * rand01();
//}

TEST(fit_predict)
{
//    using namespace ar;
    using namespace std;

    // Maximum model estimation order to perform (limited by N)
//size_t maxorder = argc > 1 && atol(argv[1]) >= 0
//                  ? atol(argv[1])
//                  : 7;

    // Default sample size shows model selection criteria differences
//  const std::size_t N = argc > 2 && atol(argv[2]) > 0
//                        ? atol(argv[2])
//                        : 10;
//
//
    // Create data to approximate from [Collomb2009]'s example.
    //
    // Notice that this isn't the most nicely conditioned problem
    // and truncating coefficients (as we'll do for display below)
    // causes roots to appear outside the unit circle.
    //vector<long double> data(N, 0.0);
    //  for (size_t i = 0; i < N; i++)
    //  {
    //      data[i] =     cos(i*0.01) + 0.75*cos(i*0.03)
    //                    + 0.5*cos(i*0.05) + 0.25*cos(i*0.11);
    //  }

    //size_t maxorder = 2;
    //const std::size_t N = 93;
    vector<double> data = {988,985.9,978.8,1000.3,995.2,977.1,976.2,988.9,983.8,996.9,996,991.8,999.4,998.9,1018.6,1009.8,990.3,987,990.2,996.7,990,984.9,996,1009.9,1004.9,1006.2,1002.3,991.6,993.1,995.8,986,1008.2,1010.3,1008.4,1009.4,1006.4,999.3,999.6,1009.2,1012.2,1005.9,1002,1007.2,994,991.8,999.6,995.3,987.8,988.6,986,978.2,979.9,983.9,972.9,971.5,977.7,993.8,997.7,1007.7,1003.6,998.7,998.9,993.8,997.6,992.5,994.4,987.4,983.9,978,991.1,997.4,997,1001.8,1013.1,1006.7,998.8,998,997.7,979.9,989.2,996.8,999.1,994,988.8,994.2,993.6,994.5,994.4,987.9,985.9,988.7,998.3,995};

    // Estimate process parameters using Burg's method
    //printf("Estimating at most an AR(%lu) model using %lu samples\n\n",
    //       static_cast<unsigned long>(maxorder),
    //       static_cast<unsigned long>(N));
//
    //bool substrac_mean = true;
    //bool hierarchy = false;
    //double mean=0;
    //vector<double> params, sigma2e, gain, autocor;
    //burg_method(data.begin(), data.end(), mean, maxorder,
    //            back_inserter(params), back_inserter(sigma2e),
    //            back_inserter(gain), back_inserter(autocor),
    //            /* subtract mean?    */ substrac_mean,
    //            /* output hierarchy? */ hierarchy);
//
    //// Display orders, mean squared discrepancy, and model coefficients
    //printf("%2s  %9s %9s %s\n", "AR", "RMS/N", "Gain", "Filter Coefficients");
    //printf("%2s  %9s %9s %s\n", "--", "-----", "----", "-------------------");
    //if(hierarchy)
    //{
    //    //for (size_t p = 0, c = 0; p <= maxorder; ++p)
    //    size_t p = 0, c = 0;
    //    {
    //        printf("%2lu  %9.2le %9.2le [ 1 ", static_cast<unsigned long>(p),
    //               sigma2e[p], gain[p]);
    //        for (size_t i = 0; i < p; ++i)
    //            printf(" %8.4lg", params[c++]);
    //        printf(" ]\n");
    //    }
//
//
    //    // Display model selection results
    //    printf("\n");
    //    vector<long double>::difference_type best;
    //    best = evaluate_models<AIC>(N, 0u, sigma2e.begin(), sigma2e.end());
    //    printf("AIC  selects model order %d as best\n", (int) best);
    //    best = evaluate_models<AICC>(N, 0u, sigma2e.begin(), sigma2e.end());
    //    printf("AICC selects model order %d as best\n", (int) best);
    //    best = evaluate_models<CIC<Burg<mean_retained> > >(
    //               N, 0u, sigma2e.begin(), sigma2e.end());
    //    printf("CIC  selects model order %d as best\n", (int) best);
    //}
    //else
    //{
    //printf("nb sigma = %d", sigma2e.size());
    // printf("%2lu  %9.2le %9.2le [ 1 ", static_cast<unsigned long>(maxorder),
    //        sigma2e[0], gain[0]);
    // for (size_t i = 0; i < params.size(); ++i)
    //     printf(" %8.4lg", params[i]);
   // printf(" ]\n");
//


    //vector<double> coeffs;
    //double Dk = BurgAlgorithm( coeffs, data, true );
    CARModel AR;
    AR.fit(2, data, true);

    printf(" %8s %8s %8s %8s %8s %8s %8s\n", "p0", "p1", "MCC", "BIC", "GIC", "AIC", "AICC");
    for (size_t i = 0; i < AR.coeffs().size(); ++i)
        printf(" %8.4lg", AR.coeffs()[i]);


    printf(" %8.4lg", AR.get_metrics<MCC>());
    printf(" %8.4lg", AR.get_metrics<BIC>());
    printf(" %8.4lg", AR.get_metrics<GIC<>>());
    printf(" %8.4lg", AR.get_metrics<AIC>());
    printf(" %8.4lg", AR.get_metrics<AICC>());
    printf("\n");

//
//
    //printf("%2lu  %9.2le %9.2le [ 1 ", static_cast<unsigned long>(maxorder),
    //       AR.mean(), AR.sigma());
//
    //for (size_t i = 0; i < AR.coeffs().size(); ++i)
    //    printf(" %8.4lg", AR.coeffs()[i]);
    //printf(" ]\n");


    size_t N2 = 10000;
    vector<double> data2 = AR.predict(N2, data);


    AR.fit(2, data2, true);

    for (size_t i = 0; i < AR.coeffs().size(); ++i)
        printf(" %8.4lg", AR.coeffs()[i]);


    printf(" %8.4lg", AR.get_metrics<MCC>());
    printf(" %8.4lg", AR.get_metrics<BIC>());
    printf(" %8.4lg", AR.get_metrics<GIC<>>());
    printf(" %8.4lg", AR.get_metrics<AIC>());
    printf(" %8.4lg", AR.get_metrics<AICC>());

    printf("\n");


    //for (size_t i=0; i < data2.size(); ++i)
    //{
//
    //    printf(" %8.4lg", data2[i]);
    //}
    /*
            std::transform(params.begin(), params.end(), params.begin(),
            std::bind1st(std::multiplies<double>(), -1.0));//to be equivalent to R result

            //double sigma2 = sigma2e[0];
            ar::predictor<double> p(params.begin(), params.end(), rand0point1 );

            std::vector<double> initial;
            for (size_t i = 0; i < params.size(); ++i)
            {
                initial.push_back(rand0point1());
            }
            p.initial_conditions(initial.begin());

            // Discard 0 <= t < burn
            for (size_t i=0; i < 500; ++i, ++p)
            {
                // NOP
            }
    //
            // Output (t, x, y, z) during burn < t <= tfinal

            vector<double> data2(N2, 0);
           // printf("\n %8.4d", data2.size());
            for (size_t i=0; i < N2; ++i, ++p)
            {
                data2[i] = mean + *p;
               // printf(" %8.4lg", data2[i]);
            }
    */
    // Estimate process parameters using Burg's method
    //  double mean2=0;
//       vector<double> params2, sigma2e2, gain2, autocor2;
//        burg_method(data2.begin(), data2.end(), mean2, maxorder,
//                   back_inserter(params2), back_inserter(sigma2e2),
//                   back_inserter(gain2), back_inserter(autocor2),
//                   /* subtract mean?    */ substrac_mean,
    //                  /* output hierarchy? */ hierarchy);

    //    printf("%2lu  %9.2le %9.2le [ 1 ", static_cast<unsigned long>(maxorder),sigma2e2[0], gain2[0]);

//    AR.fit(2, data, true);
    //      for (size_t i = 0; i < params2.size(); ++i)
    //       printf(" %8.4lg", params2[i]);
    //  printf(" ]\n");

    //}



//    return 0;
}
