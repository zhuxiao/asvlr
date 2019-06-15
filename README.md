# ASVCLR
Accurate Structural Variation Caller for Long Reads

-------------------
ASVCLR is an accurate Structural Variation Caller for Long Reads, such as PacBio sequencing and Oxford Nanopore sequencing. ASVCLR can detect both short indels (e.g. <50 bp) and long structural varitions (e.g. >=50 bp), such as duplications, inversions and translocations, and producing fewer false positives with high precise variant margins.  

## Prerequisites
ASVCLR depends on the following libraries and tools:
* HTSlib (http://www.htslib.org/download/)
* Canu (v1.7.1) (https://github.com/marbl/canu/releases/tag/v1.7.1)
* BLAT (http://hgdownload.soe.ucsc.edu/admin/exe/linux.x86_64/blat/)
* g++.

The above library and tools should be installed before compiling ASVCLR. Canu and BLAT should be globally accessible in the computer system, these executable files `canu` and `blat` should be placed or linked to the `$PATH` directory.

Note that: Canu v1.7.1 is about 5 to 10 folds faster than Canu v1.8 (https://github.com/marbl/canu/releases), however, it may could not construct the assembly results (contigs) in some genomic regions due to the overlap failure, therefore if you care more about the accuracy of the results than the running time, please use Canu v1.8 instead.

Moreover, according to our human chromosome 1 simulated results, the Recall was increased from xxx to xxx, the Precision was increased from xxx to xxx, and the F1 score was increased from xxx to xxx when using Canu v1.8 instead of v.1.7.1, which was a slight increase. Therefore, Canu v1.7.1 is recommended in ASVCLR.


## Compiling ASVCLR

The binary file can be generated by typing:
```sh
$ git clone https://github.com/zhuxiao/asvclr.git
$ cd asvclr/
$ ./autogen.sh
```
And the binary file `asvclr` will be output into the folder `bin` in this package directory.


## Quick Usage

Simply, ASVCLR can be run by typing the `all` command:
```sh
$ asvclr all -t 14 -c 20000 -f hg38.fa hg38_ngmlr_sorted.bam
```
Then, the following commands `detect`, `assemble` and `call` will be performed in turn. The help information can be shown:
```sh
$ asvclr
Program: asvclr (Accurate Structural Variation Caller for Long Reads)
Version: 0.1.0 (using htslib 1.9)

Usage:  asvclr  <command> [options]

Commands:
     detect       detect indel signatures in aligned reads
     assemble     assemble candidate regions and align assemblies
                  back to reference
     call         call indels by alignments of local genome assemblies
     all          run the above commands in turn
```


## Usage

Alternatively, there are three steps to run ASVCLR: `detect`, `assemble` and `call`.

```sh
$ asvclr detect -t 14 -c 20000 -f hg38.fa hg38_ngmlr_sorted.bam
$ asvclr assemble -t 14 -c 20000 -f hg38.fa hg38_ngmlr_sorted.bam
$ asvclr call -t 14 -c 20000 -f hg38.fa hg38_ngmlr_sorted.bam
```

The reference and an sorted BAM file will be the input of ASVCLR, and the variants stored in the BED file format and translocations in BEDPE file format will be generated as the output.


### `Detect` Step
 
Structural variant regions will be detected according to abnormal features. These regions includes insertions, deletions, duplications, inversions and translocations.

```sh
$ asvclr detect -t 14 -c 20000 -f hg38.fa hg38_ngmlr_sorted.bam
```

And the help information are shown below:

```sh
$ asvclr detect
Program: asvclr (Accurate Structural Variation Caller for Long Reads)
Version: 0.1.0 (using htslib 1.9)

Usage: asvclr detect [options] <in.bam>|<in.sam>

Options: 
     -f FILE      reference file name (required)
     -b INT       block size [1000000]
     -s INT       detect slide size [500]
     -m INT       minimal SV size to detect [2]
     -n INT       minimal clipping reads supporting a SV [7]
     -c INT       maximal clipping region size to detect [10000]
     -o FILE      prefix of the output file [stdout]
     -t INT       number of threads [0]
     -M INT       Mask mis-aligned regions [1]: 1 for yes, 0 for no
     -h           show this help message and exit
```

### `Assemble` Step

Perform local assembly for the detected variant regions using Canu, and extract the corresponding local reference.

```sh
$ asvclr assemble -t 14 -c 20000 -f hg38.fa hg38_ngmlr_sorted.bam
```

And the help information are shown below:

```sh
$ asvclr assemble
Program: asvclr (Accurate Structural Variation Caller for Long Reads)
Version: 0.1.0 (using htslib 1.9)

Usage: asvclr assemble [options] <in.bam>|<in.sam>

Options: 
     -f FILE      reference file name (required)
     -b INT       block size [1000000]
     -S INT       assemble slide size [10000]
     -c INT       maximal clipping region size to assemble [10000]
     -o FILE      prefix of the output file [stdout]
     -t INT       number of threads [0]
     -M INT       Mask mis-aligned regions [1]: 1 for yes, 0 for no
     -h           show this help message and exit

```

### `Call` Step

Align the assembly result (contigs) to its local reference using BLAT to generate the sim4 formated alignments, and call each type variations using the BLAT alignments.

```sh
$ asvclr call -t 14 -c 20000 -f hg38.fa hg38_ngmlr_sorted.bam
```

And the help information are shown below:

```sh
$ asvclr call
Program: asvclr (Accurate Structural Variation Caller for Long Reads)
Version: 0.1.0 (using htslib 1.9)

Usage: asvclr call [options] <in.bam>|<in.sam>

Options: 
     -f FILE      reference file name (required)
     -b INT       block size [1000000]
     -S INT       assemble slide size [10000]
     -c INT       maximal clipping region size to call [10000]
     -o FILE      prefix of the output file [stdout]
     -t INT       number of threads [0]
     -M INT       Mask mis-aligned regions [1]: 1 for yes, 0 for no
     -h           show this help message and exit
```


## Output Result Description

There are two kinds of output files: BED file and BEDPE file. Insertions, deletions, inversions and duplications are stored in the BED file format, and translocations are saved in the BEDPE file format.

There are 8 columns in the generated BED format file for insertions, deletions, inversions and duplications, and these columns can be described as below:

```sh
chromosome	start_ref_pos	end_ref_pos	SV_type	SV_len	Dup_num	Ref_seq	Alt_seq
```
Dup_num is the number of copies for the tandem duplications, Ref_seq is the reference sequence in variant regions, and the Alt_seq is the sample sequence in the variant regions.

For translocations, the file format should be BEDPE, and the first 13 columns can be described as below:

```sh
chromosome1	start_ref_pos1	end_ref_pos1	chromosome2	start_ref_pos2	end_ref_pos2	SV_type	SV_len1	SV_len2	Ref_seq1	Alt_seq1	Ref_seq2	Alt_seq2
```

Note that: In ASVCLR, all variant types, including translocations, can be stored together in the same BED file, for example:

```sh
chr1	3033733	3033733	INS	41	-	C	CTTGCCTCTTGGATTTCATTCCTTGGTTAGTTTCTCTCAAAA
chr1	1185621	1185630	DEL	-9	-	AGTCCTATTG	A
chr2	3270134	3270184	INV	0	-	TTCCTTAAGAAACATTGTTGTTTTTAAAGTGAATTGATTGTCGCGGTTTCT	AGAAACCGCGACAATCAATTCACTTTAAAAACAACAATGTTTCTTAAGGAA
chr1	3081820	3085608	INV	3	-	TTCTATGTCATTTTTAATT...	AAAAAGAGGCCTGGACATCAATTGA...
chr3	1079105	1079633	DUP	1055	2	AGTTAATTCATTAATACTAATACTATCG...	AGTTAATTCATTAATACTAATACTATCGAGGATT...
chr1	1000002	2005000	chr2	2000002	2005000	TRA	4999	4999	TATGAATGCCGCAGCTGGAAACTC...	AAAACGAGTTTTAGTTCAGTAGG...	AAAACGAGTTTTAGTTCAGTAGG...	TATGAATGCCGCAGCTGGAAACTC...
chr1	-	3000000	chr2	-	4000001	TRA	-	-	-	-	-	-
```
The last variant item is a translocation breakpoint whose location are 3000000 of chr1 (4000001 of chr2).

------------------

## Contact

If you have problems or some suggestions, please contact: xzhu@hrbnu.edu.cn without hesitation. 

---- Enjoy !!! -----

