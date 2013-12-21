//////////////////////////////////////////////////////////////////////////
// 
// SOURCE CODE: https://github.com/yunfuliu/pixkit
// 
// BEIRF: pixkit-image contains image processing related methods which have been published (on articles, e.g., journal or conference papers). 
//	In addition, some frequently used related tools are also involved.
// 
//////////////////////////////////////////////////////////////////////////
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cmath>
#include <iostream>
#include <vector>
#include <fstream>

#ifndef __PIXKIT_IMAGE_HPP__
#define __PIXKIT_IMAGE_HPP__

namespace pixkit{

	//////////////////////////////////////////////////////////////////////////
	/// attack
	namespace attack{

		/**
		* @brief		add Gaussian noise to each pixel
		* @bug			目前稍嫌不正確, 應將normal的cdf轉換, 以將輸入y取得x, 之公式取得, 才可進行正確運算.
		*
		* @param		sd:	standard deviation, unit: grayscale. range: 0~255
		*/
		bool	addGaussianNoise(const cv::Mat &src,cv::Mat &dst,const double sd);

		/**
		* @brief		add white noise to each pixel
		*
		* @param		maxMag: the biggest mag of the noise
		*/
		bool	addWhiteNoise(const cv::Mat &src,cv::Mat &dst,const double maxMag);

	}

	//////////////////////////////////////////////////////////////////////////
	/// filtering related
	namespace filtering{

		/**
		* @brief		filtering with median filter
		* @brief		paper: no
		*
		* @author		Yunfu Liu (yunfuliu@gmail.com)
		* @date			Sept. 6, 2013
		* @version		1.0
		*
		* @param		src: input image (grayscale only)
		* @param		dst: output image
		* @param		blocksize: the used block size
		*
		* @return		bool: true: successful, false: failure
		*/
		bool medianfilter(const cv::Mat &src,cv::Mat &dst,cv::Size blocksize);

	}


	//////////////////////////////////////////////////////////////////////////
	/// Edge detection related
	namespace edgedetection{

		/**
		* @brief		Sobel edge detection
		* @brief		paper: digital image processing textbook
		*
		* @author		Yunfu Liu (yunfuliu@gmail.com)
		* @date			Sept. 4, 2013
		* @version		1.0
		*
		* @param		src: input image (grayscale only)
		* @param		dst: output image
		*
		* @return		bool: true: successful, false: failure
		*/
		bool Sobel(const cv::Mat &src, cv::Mat &dst);
	}

	//////////////////////////////////////////////////////////////////////////
	/// Halftoning related
	namespace halftoning{

		/// Dot diffusion related
		namespace dotdiffusion{

			/**
			* @brief		paper: Yun-Fu Liu and Jing-Ming Guo, "New class tiling design for dot-diffused halftoning," IEEE Trans. Image Processing, vol. 22, no. 3, pp. 1199-1208, March 2013.
			* 
			* @param		ctSize:	CT size
			*/
			class CNADDCT{
			public:
				int				m_CT_height;	// CT's height and width
				int				m_CT_width;
				unsigned char	**m_ct;
				CNADDCT();
				~CNADDCT();
				bool			generation(cv::Size ctSize);	// ct generation
				bool			save(char name[]);
				bool			load(char name[]);
			private:
				int				m_CTmap_height;	// ct map
				int				m_CTmap_width;
				int				m_CM_size;		// class matrix
				int				m_numberOfCM;	// number of cm in a ct
				int				**m_cm;
				int				*m_cmData;
				unsigned char	*m_ctmap;
				unsigned char	*m_ctData;
			};
			void NADD2013(cv::Mat &src,cv::Mat &dst,pixkit::halftoning::dotdiffusion::CNADDCT &cct);

			/**
			* @brief		paper: J. M. Guo and Y. F. Liu"Improved dot diffusion by diffused matrix and class matrix co-optimization," IEEE Trans. Image Processing, vol. 18, no. 8, pp. 1804-1816, 2009.
			*
			* @author		Yunfu Liu (yunfuliu@gmail.com)
			* @date			May 17, 2013
			* @version		1.0
			* 
			* @param		src: input image (grayscale only)
			* @param		dst: output image
			* @param		ClassMatrixSize: allow only 8x8 and 16x16
			*
			* @return		bool: true: successful, false: failure
			*/ 
			bool GuoLiu2009(const cv::Mat &src, cv::Mat &dst,const int ClassMatrixSize);


		}

	}


	//////////////////////////////////////////////////////////////////////////
	/// Image enhancement related
	namespace enhancement{
		
		/// Local methods
		namespace local{

			/**
			* @brief		local contrast enhancement
			* @brief		paper: B. Liu, W. Jin, Y. Chen, C. Liu, and L. Li, "Contrast enhancement using non-overlapped sub-blocks and local histogram projection," TCE, vol. 57, no. 2, 2011.
			* @brief		nickname: non-overlapped sub-blocks and local histogram projection based contrast enhancement (NOSHP)
			*
			* @author		Yunfu Liu
			* @date			Sept. 3, 2013
			* @version		1.0
			*
			* @param		N: number of blocks. (due to this value will be divided by image size, so it can be regarded as 'block size')
			*
			* @return		bool: true: successful, false: failure
			*/
			bool LiuJinChenLiuLi2011(const cv::Mat &src,cv::Mat &dst,const cv::Size N);

			/**
			* @brief		local contrast enhancement		
			* @brief		paper: L. Jiao, Z. Sun, and A. Sha, "Improvement of image contrast with local adaptation,," Intl. Conf. Multimedia and Information Technology, 2010.
			* @brief		nickname: Partially Overlapped Sub-block Logarithmic Trandformation (POSLT)
			*
			* @author		Yunfu Liu (yunfuliu@gmail.com)
			* @date			Sept. 3, 2013
			* @bug			This function still has some bugs. Nov. 28, 2013.
			* 
			* @param		BlockSize: block size (blocksize >= step size)
			* @param		StepSize: step size (the smaller step size, the better quality; when step size = block size: nonoverlapped strategy; when step size< block: overlapped strategy)
			* @param		mode: 1: use EME as cost function; 2: use TEN as cost function
			*
			* @return		bool: true: successful, false: failure
			*/
			bool JiaoSunSha2010(const cv::Mat &src,cv::Mat &dst,const cv::Size BlockSize,const cv::Size StepSize,const short mode);

			/**
			* @brief		local contrast enhancement, KimKimHwang2001 POSHE's improvement
			* @brief		paper: F. Lamberti, B. Montrucchio, and A. Sanna, "CMBFHE: a novel contrast enhancement technique based on cascaded multistep binomial filtering histogram equalization," TCE, vol. 52, no. 3, 2006.
			* @brief		nickname: cascaded multistep binomial filtering histogram equalization (CMBFHE)
			*
			* @author		賴柏勳, Yunfu Liu
			* @date			May 15, 2013
			*
			* @param: B: number of blocks
			* @param: S: number of regions divided by the step size. S should >2xB. .This should be the "Bx2" or "Bx4" or "Bx8" etc multiple of 2. "S" should be B's power (i.e., 2,4,8,16,32,64,128...). Bigger S, better image quality, and slower.
			*
			* @return: bool: true: successful, false: failure
			*/
			bool LambertiMontrucchioSanna2006(const cv::Mat &src,cv::Mat &dst,const cv::Size B,const cv::Size S);
			
			
			/**
			* @brief		local contrast enhancement
			* @brief		paper: Z. Yu and C. Bajaj, "A fast and adaptive method for image contrast enhancement," ICIP, vol. 2, pp. 1001-1004, 2004.
			*
			* @author		Yunfu Liu (yunfuliu@gmail.com)
			* @date			May 13, 2013
			*
			* @param		C: within [0,1] - this is only for isotropic mode
			* @param		anisotropicMode: false-isotropic mode; true-anisotropic mode (iso needs C; ani needs R, respectively)
			* @param		R: within [0.01,0.1] - this is only for anisotropic mode
			*
			* @return		bool: true: successful, false: failure
			*/ 
			bool YuBajaj2004(const cv::Mat &src,cv::Mat &dst,const int blockheight,const int blockwidth,const float C=0.85f,bool anisotropicMode=false,const float R=0.09f);

			/**
			* @brief		local contrast enhancement
			* @brief		paper: J. Y. Kim, L. S. Kim, and S. H. Hwang, "An advanced contrast enhancement using partially overlapped sub-block histogram equalization," TCSVT, vol. 11, no. 4, pp. 475-484, 2001. 
			* @brief		nickname: partially overlapped sub-block histogram equalization (POSHE)
			* 
			* @author		Yunfu Liu (yunfuliu@gmail.com)
			* @date			May 15, 2013
			*
			* @param		B: number of blocks
			* @param		S: number of regions divided by the step size. S should >2xB. .This should be the "Bx2" or "Bx4" or "Bx8" etc multiple of 2. "S" should be B's power (i.e., 2,4,8,16,32,64,128...). Bigger S, better image quality, and slower.
			*
			* @return		bool: true: successful, false: failure
			*/
			bool KimKimHwang2001(const cv::Mat &src,cv::Mat &dst,const cv::Size B,const cv::Size S);

			/**
			* @brief		local contrast enhancement
			* @brief		paper: J. A. Stark, "Adaptive image contrast enhancement using generalizations of histogram equalization," TIP, vol. 9, no. 5, pp. 889-896, 2000.
			* 
			* @author		賴柏勳, Yunfu Liu 
			* @date			May 14, 2013
			* 
			* @param		alpha: 0~1. 0: histogram equalization; 1: local-mean subtraction effect.
			* @param		beta: 0~1
			* 
			* @return		bool: true: successful, false: failure
			*/
			bool Stark2000(const cv::Mat &src,cv::Mat &dst,const int blockheight,const int blockwidth,const float alpha=0.5f,const float beta=0.5f);


			/**
			* @brief		local contrast enhancement
			* @brief		paper: R. C. Gonzalez and R. E. Woods, Digital Image Processing, 2nd ed., Reading, MA: Addison-Wesley, 1992.
			* @brief		nickname: local histogram equalization (LHE)
			*
			* @author		Yunfu Liu (yunfuliu@gmail.com)
			* @date			May 16, 2013
			* 
			* @param		blocksize: block sizes
			* 
			* @return		bool: true: successful, false: failure
			*/
			bool LocalHistogramEqualization1992(const cv::Mat &src,cv::Mat &dst,const cv::Size blocksize);
		}

		/// Global methods
		namespace global{

			/**
			* @brief		global contrast enhancement
			* @brief		paper: M. Abdullah-Al-Wadud, Md. Hasanul Kabir, M. Ali Akber Dewan, and O. Chae, "A dynamic histogram equalization for image contrast enhancement," Intl. Conf. Consumer Electronics, pp. 1-2, 2007.
			* @brief		nickname: dynamic histogram equalization (DHE)
			* 
			* @author		劉少雲
			* @date			May 15, 2013
			* @bug			It still has some bugs.
			* 
			* @param		x: ??????????????????????????????????
			* 
			* @return		bool: true: successful, false: failure
			*/
			bool WadudKabirDewanChae2007(const cv::Mat &src, cv::Mat &dst, const int x);

			/**
			* @brief		global contrast enhancement
			* @brief		paper: R. C. Gonzalez and R. E. Woods, Digital Image Processing, 2nd ed., Reading, MA: Addison-Wesley, 1992.
			* @brief		nickname: global histogram equalization (GHE)
			* 
			* @author		Yunfu Liu (yunfuliu@gmail.com)
			* @date			May 16, 2013
			* 
			* @return		bool: true: successful, false: failure
			*/
			bool GlobalHistogramEqualization1992(const cv::Mat &src,cv::Mat &dst);

		}

	}

	//////////////////////////////////////////////////////////////////////////
	/// IQA related 
	namespace qualityassessment{

		/**
		* @brief		usually used to estimate contrast of an image, and it can evaluate naturalness and uniform lighting 
		* @brief		paper: original: S. S. Agaian, K. Panetta, and A. M. Grigoryan, "A new measure of image enhancement," in Proc. Intl. Conf. Signal Processing Communication, 2000.
		* @brief		another representation: (this is the one used in this implementation) S. S. Agaian, B. Silver, and K. A. Panetta, "Transform coefficient histogram-based image enhancement algorithms using contrast entropy," TIP, 2007. 
		* @brief		nickname: measure of enhancement (EME) or measure of improvement
		* 
		* @author		Yunfu Liu (yunfuliu@gmail.com)
		* @date			Sept. 4, 2013
		*
		* @param		nBlocks: number of blocks at either x or y axis; this size should be odd since this is just like a filter as defined in the paper
		* @param		mode: 1: standard mode: use the local max and min to evaluate the eme; 2: ab mode: use BTC's a and b to represent a block's contrast
		*
		* @return		float: return the value of EME
		*/
		float EME(const cv::Mat &src,const cv::Size nBlocks,const short mode=1);

		/**
		* @brief		it is able to describe whether some artificial texture appear or not
		* @brief		paper: L. Jiao, Z. Sun, and A. Sha, "Improvement of image contrast with local adaptation," in Proc. Intl. Conf. Multimedia and Informatin Technology, 2010.
		* @brief		(this paper used this TEN in their paper, and the original one is published in 1970 as in their reference list)
		* @brief		nickname: TEN
		*
		* @author		Yunfu Liu (yunfuliu@gmail.com)
		* @date			Sept. 4, 2013
		*
		* @return		float: return the value of TEN
		*/
		float TEN(const cv::Mat &src);

		/**
		* @brief		used to estimate the difference between two images (!!!the lower the better)
		* @brief		paper: N. Phanthuna, F. Cheevasuvit, and S. Chitwong, "Contrast enhancement for minimum mean brightness error from histogram partitioning," ASPRS Conf. 2009.
		* @brief		nickname: absolute mean brightness error (AMBE)
		*
		* @author		Yunfu Liu (yunfuliu@gmail.com)
		* @date			Sept. 4, 2013
		*
		* @return		float: return the value of AMBE
		*/
		float AMBE(const cv::Mat &src1,const cv::Mat &src2);


		/**
		* @brief		derive PSNR
		*/
		float	PSNR(const cv::Mat &src1,const cv::Mat &src2);

	}

}
#endif