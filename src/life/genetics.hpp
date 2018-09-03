#ifndef genetics_hpp
#define genetics_hpp

// The class representing the base of DNA.
class Nucleotide
{
public:
    Nucleotide();
    ~Nucleotide();

    // Nucleotides of a matching pair should add up to 3. Represent Nucleotides as binary numbers so space can be saved.
    enum NTYPE
    {
        ADENINE = 0x00,
        CYTOSINE = 0x01,
        GUANINE = 0x02,
        THYMINE = 0x03,
    };
};

// This is a class that represents the DNA of creatures.
class DNA
{
public:
    DNA();
    ~DNA();
    std::vector< std::shared_ptr<Nucleotide> > leftHelix;
    std::vector< std::shared_ptr<Nucleotide> > rightHelix;
};

// This class represents RNA, for use in CRISPR and Viruses.
class RNA
{
public:
    RNA();
    ~RNA();
    std::vector< std::shared_ptr<Nucleotide> > helix;
};

// A small segment of code that is made of interlocking acids. It represents a set of DNA that actually does something.
class Gene
{
public:
    Gene();
    ~Gene();
    // Starting and ending index of the DNA, and the chromosome it belongs to.
    int dnaStart, dnaEnd, chromNum;
};

// This class represents a coiled set of DNA.
class Chromosome
{
public:
    Chromosome();
    ~Chromosome();
    std::shared_ptr<DNA> dna;
};

class Nucleus
{
public:
    Nucleus();
    ~Nucleus();
    std::vector< std::shared_ptr<Chromosome> > chromosomes;
};

class ModelCell
{
public:
    ModelCell();
    ~ModelCell();

    virtual void readDNA() = 0;
    std::shared_ptr<Nucleus> nucleus;
};

class AnimalCell : public ModelCell
{
public:
    AnimalCell();
    ~AnimalCell();

    void readDNA();
};

#endif