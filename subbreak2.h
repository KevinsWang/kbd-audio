/*! \file subbreak2.h
 *  \brief Breaking non-exact substitution ciphers
 *  \author Georgi Gerganov
 */

#pragma once

#include "common.h"

#include <map>
#include <cmath>
#include <vector>
#include <string>

namespace Cipher {

    using TCode = int32_t;
    using TProb = double;
    using TGramLen = int32_t;

    struct TParameters {
        // clustering params
        int minClusters = 23;
        int maxClusters = 30;
        int nIterationsPerUpdate = 100000;
        int nChangePerIteration = 1;

        // simulated annealing params
        int saMaxIterations = 20000000;
        float temp0 = 1e0;
        float coolingRate = 0.999995;

        // subbreak params
        int nSubbreakIterations = 100000;

        // language model
        float wEnglishFreq = 10.0f;
        float wLanguageModel = 1.0f;

        // simulation parameters
        float waveformDeviationMin = 0.1f;
        float waveformDeviationSig = 0.2f;
        float waveformDetectionErrorP = 0.10f;
        float waveformDetectionErrorMin = 0.5f;
        float waveformDetectionErrorSig = 0.4f;
        float similarityNoiseSig = 0.1f;
        float similarityMismatchAvg = 0.3f;
        float similarityMismatchSig = 0.2f;
    };

    struct TFreqMap {
        TGramLen len;
        TProb pmin;
        std::vector<TProb> prob;
    };

    TCode calcCode(const char * data, int n);
    bool loadFreqMap(const char * fname, TFreqMap & res);

    bool encryptExact(const TParameters & params, const std::string & text, TClusters & clusters);
    bool generateSimilarityMap(const TParameters & params, const std::string & text, TSimilarityMap & ccMap);

    bool generateClusters(const TParameters & params, int n, TClusters & clusters, const std::vector<int> & hint);
    bool printClusterGoodness(const std::string & text, const TClusters & clusters);

    float costF(const TSimilarityMap & ccMap, const TClusters & clusters);
    float costFUpdate(const TSimilarityMap & ccMap, const TClusters & clusters, int i, int cid, float c0);

    bool doSimulatedAnnealing3(
            const TParameters & params,
            const TSimilarityMap & ccMap,
            TClusters & clusters,
            const std::vector<int> & hint = {}
            );

    bool doSimulatedAnnealing4(
            const TParameters & params,
            const TFreqMap & freqMap,
            const TClusters & clusters,
            TClusterToLetterMap & clMap,
            const std::vector<int> & hint = {}
            );

    bool doSimulatedAnnealing5(
            const TParameters & params,
            const TFreqMap & freqMap,
            const TSimilarityMap & ccMap,
            TClusters & clusters,
            TClusterToLetterMap & clMap,
            const std::vector<int> & hint = {}
            );

    bool subbreak(
            const TParameters & params,
            const TFreqMap & freqMap,
            const TClusters & clusters,
            TClusterToLetterMap & clMap,
            const std::vector<int> & hint = {}
            );

    bool generateClustersInitialGuess(
            const TParameters & params,
            const TSimilarityMap & ccMap,
            TClusters & clusters);

    bool mutateClusters(const TParameters & params, TClusters & clusters);

    float calcPClusters(
            const TParameters & ,
            const TSimilarityMap & ,
            const TSimilarityMap & logMap,
            const TSimilarityMap & logMapInv,
            const TClusters & clusters);

    bool normalizeSimilarityMap(
            const TParameters & ,
            TSimilarityMap & ccMap,
            TSimilarityMap & logMap,
            TSimilarityMap & logMapInv);

    void printText(const TClusters & t);
    void printText(const TClusters & t, const TClusterToLetterMap & clMap);
}
