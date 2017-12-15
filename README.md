# PVCTools
Parallel Variation Calling Tools

PVCTools is Parallel Variation Calling Tools, which tries to call variation using multiple threads. Basically, the reference genome will be splited into small pieces, and corresponding alignment BAM files will be extracted. Under this way, it will speed up the process of variation calling a lot.


Now, we have tested on LSF system.



Prerequisites

  Samtools: http://www.htslib.org/doc/samtools.html
  
  GATK: https://software.broadinstitute.org/gatk/
  
  Picard tools: http://broadinstitute.github.io/picard/
  
  bamUtil: http://genome.sph.umich.edu/wiki/BamUtil
  
  bcftools: https://samtools.github.io/bcftools/bcftools.html
  
  freebayes: https://github.com/ekg/freebayes


Install

To install PVCTools, please follow:
   unzip PVCTools_alpha_v*.zip
   make





