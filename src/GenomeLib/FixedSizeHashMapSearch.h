#ifndef FIXEDSIZEHASHMAPSEARCH_H_
#define FIXEDSIZEHASHMAPSEARCH_H_

#include "BrainTwister/ArgumentParser.h"
#include "GenomeLib/FullMatchManager.h"
#include "GenomeLib/Genome.h"
#include "GenomeLib/Match.h"
#include "GenomeLib/NucleotideDatabaseIterator.h"
#include "UtilitiesLib/Environment.h"
#include "UtilitiesLib/FileIO.h"
#include "UtilitiesLib/Filesystem.h"
#include "UtilitiesLib/StringUtilities.h"
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <mutex>
#include <thread>
#include <vector>

std::mutex mutex;

namespace GeneHunter {

template <class T>
void threadFunction( FullMatchManager& fullMatchManager, FASTQIterator& iterEntryCur, FASTQIterator const& iterEntryEnd,
    size_t& countRead, size_t maxReads, size_t step, NucleotideDatabaseInformation& readInfo, NucleotideDatabase<T> const& db )
{
    mutex.lock();
    for ( ; iterEntryCur != iterEntryEnd and countRead != maxReads; ++iterEntryCur, ++countRead )
    {
        FASTQ read(**iterEntryCur);
        if (step == 1) readInfo.add(read.getSequence().getNbBases());

        Match match = fullMatchManager.getMatch(read.getName());

        mutex.unlock();
        fullMatchManager.findBestMatch(db,read,match);
        mutex.lock();

        fullMatchManager.addMatch(read.getName(),match);
    }
    mutex.unlock();
}

template <FixedTokenSizeType Size>
void fixedSizeHashMapSearchImpl( FullMatchManager& fullMatchManager, NucleotideDatabaseSettings const& nucleotideDatabaseSettings, BrainTwister::ArgumentParser const& arg )
{
    using namespace std;

    auto readFile = arg.get<filesystem::path>("read");
    if (!exists(readFile)) throw GeneHunterException("File not found: " + readFile.string());

    auto nt_file = arg.get<filesystem::path>("nt");
    if (!exists(nt_file)) throw GeneHunterException("File not found: " + nt_file.string());
    std::ifstream ifs(nt_file.string());
    if (!ifs) throw GeneHunterException("Error opening file " + nt_file.string());
    boost::iostreams::filtering_streambuf<boost::iostreams::input> inbuf;
    if (nt_file.extension() == ".gz") inbuf.push(boost::iostreams::gzip_decompressor());
    inbuf.push(ifs);
    boost::shared_ptr<std::istream> ptr_instream(new std::istream(&inbuf));

    size_t maxEntries = arg.get<size_t>("maxEntries");
    size_t maxBases = arg.get<size_t>("maxBases");
    size_t maxBasesPerStep = arg.get<size_t>("maxBasesPerStep");
    size_t maxReads = arg.get<size_t>("maxReads");
    size_t startEntry = arg.get<size_t>("startEntry");

    NucleotideDatabaseInformation totalInfo;
    NucleotideDatabaseInformation readInfo;

    ofstream hashMapInfoFile;
    if (arg.get<bool>("printHashMapInfo")) hashMapInfoFile.open("NucleotideDatabaseHashMapInformation.txt");

    size_t step(1);
    size_t nbCoresPerNode = Environment::getNbCoresPerNode();
    for (NucleotideDatabaseIterator<Traits<Size> > iterNTDBCur(ptr_instream, maxEntries,
    	maxBases, maxBasesPerStep, startEntry, nucleotideDatabaseSettings),
        iterNTDBEnd; iterNTDBCur != iterNTDBEnd; ++iterNTDBCur, ++step )
    {
        totalInfo.merge((*iterNTDBCur)->getInformation());

        if (arg.get<bool>("printSteps")) {
            string header = " Intermediate report import informations for step " + boost::lexical_cast<string>(step);
            cout << endl << header << endl << string(header.size()+1,'-') << endl << (*iterNTDBCur)->getInformation() << endl;
        }

        if (arg.get<bool>("printHashMapInfo")) {
            hashMapInfoFile << underline("NucleotideDatabaseHashMapInformation for step " + boost::lexical_cast<string>(step) + " " ) << endl;
            (*iterNTDBCur)->printHashMapReferenceSequenceState(hashMapInfoFile);
            hashMapInfoFile << endl;

            hashMapInfoFile << underline("Top five tokens: ") << endl;
            (*iterNTDBCur)->printMostFrequentOccurrences(hashMapInfoFile,5);
            hashMapInfoFile << endl;
        }

        size_t countRead(0);
        if ( nbCoresPerNode == 0 ) {
            for ( FASTQIterator iterEntryCur(readFile), iterEntryEnd;
                iterEntryCur != iterEntryEnd and countRead != maxReads; ++iterEntryCur, ++countRead )
            {
                if (step == 1) readInfo.add((*iterEntryCur)->getSequence().getNbBases());
                fullMatchManager.includeBestMatch(**iterNTDBCur,**iterEntryCur);
            }
        } else {
            vector< shared_ptr<thread> > threads(nbCoresPerNode);

            FASTQIterator iterEntryCur(readFile), iterEntryEnd;

            for ( auto & singleThread : threads ) {
                singleThread.reset(new thread(threadFunction<Traits<Size>>,
                    std::ref(fullMatchManager),
                    std::ref(iterEntryCur),
                    std::ref(iterEntryEnd),
                    std::ref(countRead),
                    maxReads,step,ref(readInfo),ref(**iterNTDBCur)));
            }

            for ( auto & thread : threads ) thread->join();
        }

        if (step == 1) {
            string header = " Summary read report import informations";
            cout << endl << header << endl << string(header.size()+1,'-') << endl << readInfo << endl;
            fullMatchManager.setTotalNbImportedReadSeq(readInfo.getNbImportedGeneSeq());
        }

        cout << "Write binary FullMatchManager to file ... " << flush;
        writeBinary(fullMatchManager,"FullMatchManager","FullMatchManager.bin");
        cout << "done." << endl;

        cout << "Write xml FullMatchManager to file ... " << flush;
        writeXML(fullMatchManager,"FullMatchManager","FullMatchManager.xml");
        cout << "done." << endl;
    }

    string header = " Summary nucleotide report import informations";
    cout << endl << header << endl << string(header.size()+1,'-') << endl << totalInfo << endl;
}

void fixedSizeHashMapSearch( FixedTokenSizeType fixedTokenSize, FullMatchManager& fullMatchManager,
    NucleotideDatabaseSettings const& nucleotideDatabaseSettings, BrainTwister::ArgumentParser const& arg )
{
    if (fixedTokenSize == 12 ) {
        fixedSizeHashMapSearchImpl<12>(fullMatchManager,nucleotideDatabaseSettings,arg);
    } else if (fixedTokenSize == 16 ) {
        fixedSizeHashMapSearchImpl<16>(fullMatchManager,nucleotideDatabaseSettings,arg);
    } else if (fixedTokenSize == 20 ) {
        fixedSizeHashMapSearchImpl<20>(fullMatchManager,nucleotideDatabaseSettings,arg);
    } else if (fixedTokenSize == 24 ) {
        fixedSizeHashMapSearchImpl<24>(fullMatchManager,nucleotideDatabaseSettings,arg);
    } else if (fixedTokenSize == 36 ) {
        fixedSizeHashMapSearchImpl<36>(fullMatchManager,nucleotideDatabaseSettings,arg);
    } else if (fixedTokenSize == 48 ) {
        fixedSizeHashMapSearchImpl<48>(fullMatchManager,nucleotideDatabaseSettings,arg);
    } else
        throw GeneHunterException("fixedTokenSize not supported.");
}

} // namespace GeneHunter

#endif /* FIXEDSIZEHASHMAPSEARCH_H_ */
