#ifndef JPEGLIB_H
#define JPEGLIB_H


#ifndef JCONFIG_INCLUDED	/* in case jinclude.h already did */
#include "jconfig.h"		/* widely used configuration options */
#endif
#include "jmorecfg.h"		/* seldom changed options */


#ifdef __cplusplus
#ifndef DONT_USE_EXTERN_C
extern "C" {
#endif
#endif

#define DCTSIZE		    8	/* The basic DCT block is 8x8 samples */
#define DCTSIZE2	    64	/* DCTSIZE squared; # of elements in a block */
#define NUM_QUANT_TBLS      4	/* Quantization tables are numbered 0..3 */
#define NUM_HUFF_TBLS       4	/* Huffman tables are numbered 0..3 */
#define NUM_ARITH_TBLS      16	/* Arith-coding tables are numbered 0..15 */
#define MAX_COMPS_IN_SCAN   4	/* JPEG limit on # of components in one scan */
#define MAX_SAMP_FACTOR     4	/* JPEG limit on sampling factors */

#define C_MAX_BLOCKS_IN_MCU   10 /* compressor's limit on blocks per MCU */
#ifndef D_MAX_BLOCKS_IN_MCU
#define D_MAX_BLOCKS_IN_MCU   10 /* decompressor's limit on blocks per MCU */
#endif

typedef JSAMPLE FAR *JSAMPROW;	/* ptr to one image row of pixel samples. */
typedef JSAMPROW *JSAMPARRAY;	/* ptr to some rows (a 2-D sample array) */
typedef JSAMPARRAY *JSAMPIMAGE;	/* a 3-D sample array: top index is color */

typedef JCOEF JBLOCK[DCTSIZE2];	/* one block of coefficients */
typedef JBLOCK FAR *JBLOCKROW;	/* pointer to one row of coefficient blocks */
typedef JBLOCKROW *JBLOCKARRAY;		/* a 2-D array of coefficient blocks */
typedef JBLOCKARRAY *JBLOCKIMAGE;	/* a 3-D array of coefficient blocks */

typedef JCOEF FAR *JCOEFPTR;	/* useful in a couple of places */



	typedef struct {

		UINT16 quantval[DCTSIZE2];	/* quantization step for each coefficient */

		boolean sent_table;		/* TRUE when table has been output */
	} JQUANT_TBL;

	typedef struct {
		UINT8 bits[17];		/* bits[k] = # of symbols with codes of */
		UINT8 huffval[256];		/* The symbols, in order of incr code length */

		boolean sent_table;		/* TRUE when table has been output */
	} JHUFF_TBL;

	typedef struct {

		int component_id;		/* identifier for this component (0..255) */
		int component_index;		/* its index in SOF or cinfo->comp_info[] */
		int h_samp_factor;		/* horizontal sampling factor (1..4) */
		int v_samp_factor;		/* vertical sampling factor (1..4) */
		int quant_tbl_no;		/* quantization table selector (0..3) */

		int dc_tbl_no;		/* DC entropy table selector (0..3) */
		int ac_tbl_no;		/* AC entropy table selector (0..3) */

		JDIMENSION width_in_blocks;
		JDIMENSION height_in_blocks;

		int DCT_h_scaled_size;
		int DCT_v_scaled_size;

		JDIMENSION downsampled_width;	 /* actual width in samples */
		JDIMENSION downsampled_height; /* actual height in samples */

		boolean component_needed;	/* do we need the value of this component? */

		int MCU_width;		/* number of blocks per MCU, horizontally */
		int MCU_height;		/* number of blocks per MCU, vertically */
		int MCU_blocks;		/* MCU_width * MCU_height */
		int MCU_sample_width;		/* MCU width in samples, MCU_width*DCT_scaled_size */
		int last_col_width;		/* # of non-dummy blocks across in last MCU */
		int last_row_height;		/* # of non-dummy blocks down in last MCU */

		JQUANT_TBL * quant_table;

		void * dct_table;
	} jpeg_component_info;

	typedef struct {
		int comps_in_scan;		/* number of components encoded in this scan */
		int component_index[MAX_COMPS_IN_SCAN]; /* their SOF/comp_info[] indexes */
		int Ss, Se;			/* progressive JPEG spectral selection parms */
		int Ah, Al;			/* progressive JPEG successive approx. parms */
	} jpeg_scan_info;

	typedef struct jpeg_marker_struct FAR * jpeg_saved_marker_ptr;

	struct jpeg_marker_struct {
		jpeg_saved_marker_ptr next;	/* next in list, or NULL */
		UINT8 marker;			/* marker code: JPEG_COM, or JPEG_APP0+n */
		unsigned int original_length;	/* # bytes of data in the file */
		unsigned int data_length;	/* # bytes of data saved at data[] */
		JOCTET FAR * data;		/* the data contained in the marker */
		/* the marker length word is not counted in data_length or original_length */
	};

	typedef enum {
		JCS_UNKNOWN,		/* error/unspecified */
		JCS_GRAYSCALE,		/* monochrome */
		JCS_RGB,		/* red/green/blue */
		JCS_YCbCr,		/* Y/Cb/Cr (also known as YUV) */
		JCS_CMYK,		/* C/M/Y/K */
		JCS_YCCK		/* Y/Cb/Cr/K */
	} J_COLOR_SPACE;

	typedef enum {
		JDCT_ISLOW,		/* slow but accurate integer algorithm */
		JDCT_IFAST,		/* faster, less accurate integer method */
		JDCT_FLOAT		/* floating-point: accurate, fast on fast HW */
	} J_DCT_METHOD;

#ifndef JDCT_DEFAULT		/* may be overridden in jconfig.h */
#define JDCT_DEFAULT  JDCT_ISLOW
#endif
#ifndef JDCT_FASTEST		/* may be overridden in jconfig.h */
#define JDCT_FASTEST  JDCT_IFAST
#endif

	typedef enum {
		JDITHER_NONE,		/* no dithering */
		JDITHER_ORDERED,	/* simple ordered dither */
		JDITHER_FS		/* Floyd-Steinberg error diffusion dither */
	} J_DITHER_MODE;


	/* Common fields between JPEG compression and decompression master structs. */

#define jpeg_common_fields \
	struct jpeg_error_mgr * err;	/* Error handler module */\
	struct jpeg_memory_mgr * mem;	/* Memory manager module */\
	struct jpeg_progress_mgr * progress; /* Progress monitor, or NULL if none */\
	void * client_data;		/* Available for use by application */\
	boolean is_decompressor;	/* So common code can tell which is which */\
	int global_state		/* For checking call sequence validity */


	struct jpeg_common_struct {
		jpeg_common_fields;		/* Fields common to both master struct types */
	};

	typedef struct jpeg_common_struct * j_common_ptr;
	typedef struct jpeg_compress_struct * j_compress_ptr;
	typedef struct jpeg_decompress_struct * j_decompress_ptr;

	struct jpeg_compress_struct {
		jpeg_common_fields;		/* Fields shared with jpeg_decompress_struct */

		struct jpeg_destination_mgr * dest;

		JDIMENSION image_width;	/* input image width */
		JDIMENSION image_height;	/* input image height */
		int input_components;		/* # of color components in input image */
		J_COLOR_SPACE in_color_space;	/* colorspace of input image */

		double input_gamma;		/* image gamma of input image */



		unsigned int scale_num, scale_denom; /* fraction by which to scale image */

		JDIMENSION jpeg_width;	/* scaled JPEG image width */
		JDIMENSION jpeg_height;	/* scaled JPEG image height */


		int data_precision;		/* bits of precision in image data */

		int num_components;		/* # of color components in JPEG image */
		J_COLOR_SPACE jpeg_color_space; /* colorspace of JPEG image */

		jpeg_component_info * comp_info;

		JQUANT_TBL * quant_tbl_ptrs[NUM_QUANT_TBLS];
		int q_scale_factor[NUM_QUANT_TBLS];


		JHUFF_TBL * dc_huff_tbl_ptrs[NUM_HUFF_TBLS];
		JHUFF_TBL * ac_huff_tbl_ptrs[NUM_HUFF_TBLS];
		/* ptrs to Huffman coding tables, or NULL if not defined */

		UINT8 arith_dc_L[NUM_ARITH_TBLS]; /* L values for DC arith-coding tables */
		UINT8 arith_dc_U[NUM_ARITH_TBLS]; /* U values for DC arith-coding tables */
		UINT8 arith_ac_K[NUM_ARITH_TBLS]; /* Kx values for AC arith-coding tables */

		int num_scans;		/* # of entries in scan_info array */
		const jpeg_scan_info * scan_info; /* script for multi-scan file, or NULL */


		boolean raw_data_in;		/* TRUE=caller supplies downsampled data */
		boolean arith_code;		/* TRUE=arithmetic coding, FALSE=Huffman */
		boolean optimize_coding;	/* TRUE=optimize entropy encoding parms */
		boolean CCIR601_sampling;	/* TRUE=first samples are cosited */
		boolean do_fancy_downsampling; /* TRUE=apply fancy downsampling */
		int smoothing_factor;		/* 1..100, or 0 for no input smoothing */
		J_DCT_METHOD dct_method;	/* DCT algorithm selector */


		unsigned int restart_interval; /* MCUs per restart, or 0 for no restart */
		int restart_in_rows;		/* if > 0, MCU rows per restart interval */

		boolean write_JFIF_header;	/* should a JFIF marker be written? */
		UINT8 JFIF_major_version;	/* What to write for the JFIF version number */
		UINT8 JFIF_minor_version;

		UINT8 density_unit;		/* JFIF code for pixel size units */
		UINT16 X_density;		/* Horizontal pixel density */
		UINT16 Y_density;		/* Vertical pixel density */
		boolean write_Adobe_marker;	/* should an Adobe marker be written? */

		JDIMENSION next_scanline;	/* 0 .. image_height-1  */

	
		boolean progressive_mode;	/* TRUE if scan script uses progressive mode */
		int max_h_samp_factor;	/* largest h_samp_factor */
		int max_v_samp_factor;	/* largest v_samp_factor */

		int min_DCT_h_scaled_size;	/* smallest DCT_h_scaled_size of any component */
		int min_DCT_v_scaled_size;	/* smallest DCT_v_scaled_size of any component */

		JDIMENSION total_iMCU_rows;	/* # of iMCU rows to be input to coef ctlr */

		int comps_in_scan;		/* # of JPEG components in this scan */
		jpeg_component_info * cur_comp_info[MAX_COMPS_IN_SCAN];

		JDIMENSION MCUs_per_row;	/* # of MCUs across the image */
		JDIMENSION MCU_rows_in_scan;	/* # of MCU rows in the image */

		int blocks_in_MCU;		/* # of DCT blocks per MCU */
		int MCU_membership[C_MAX_BLOCKS_IN_MCU];


		int Ss, Se, Ah, Al;		/* progressive JPEG parameters for scan */

		struct jpeg_comp_master * master;
		struct jpeg_c_main_controller * main;
		struct jpeg_c_prep_controller * prep;
		struct jpeg_c_coef_controller * coef;
		struct jpeg_marker_writer * marker;
		struct jpeg_color_converter * cconvert;
		struct jpeg_downsampler * downsample;
		struct jpeg_forward_dct * fdct;
		struct jpeg_entropy_encoder * entropy;
		jpeg_scan_info * script_space; /* workspace for jpeg_simple_progression */
		int script_space_size;
	};


	struct jpeg_decompress_struct {
		jpeg_common_fields;		/* Fields shared with jpeg_compress_struct */

		/* Source of compressed data */
		struct jpeg_source_mgr * src;


		JDIMENSION image_width;	/* nominal image width (from SOF marker) */
		JDIMENSION image_height;	/* nominal image height */
		int num_components;		/* # of color components in JPEG image */
		J_COLOR_SPACE jpeg_color_space; /* colorspace of JPEG image */



		J_COLOR_SPACE out_color_space; /* colorspace for output */

		unsigned int scale_num, scale_denom; /* fraction by which to scale image */

		double output_gamma;		/* image gamma wanted in output */

		boolean buffered_image;	/* TRUE=multiple output passes */
		boolean raw_data_out;		/* TRUE=downsampled data wanted */

		J_DCT_METHOD dct_method;	/* IDCT algorithm selector */
		boolean do_fancy_upsampling;	/* TRUE=apply fancy upsampling */
		boolean do_block_smoothing;	/* TRUE=apply interblock smoothing */

		boolean quantize_colors;	/* TRUE=colormapped output wanted */
		/* the following are ignored if not quantize_colors: */
		J_DITHER_MODE dither_mode;	/* type of color dithering to use */
		boolean two_pass_quantize;	/* TRUE=use two-pass color quantization */
		int desired_number_of_colors;	/* max # colors to use in created colormap */
		/* these are significant only in buffered-image mode: */
		boolean enable_1pass_quant;	/* enable future use of 1-pass quantizer */
		boolean enable_external_quant;/* enable future use of external colormap */
		boolean enable_2pass_quant;	/* enable future use of 2-pass quantizer */

		JDIMENSION output_width;	/* scaled image width */
		JDIMENSION output_height;	/* scaled image height */
		int out_color_components;	/* # of color components in out_color_space */
		int output_components;	/* # of color components returned */
		/* output_components is 1 (a colormap index) when quantizing colors;
		* otherwise it equals out_color_components.
		*/
		int rec_outbuf_height;	/* min recommended height of scanline buffer */

		int actual_number_of_colors;	/* number of entries in use */
		JSAMPARRAY colormap;		/* The color map as a 2-D pixel array */

		JDIMENSION output_scanline;	/* 0 .. output_height-1  */

		int input_scan_number;	/* Number of SOS markers seen so far */
		JDIMENSION input_iMCU_row;	/* Number of iMCU rows completed */


		int output_scan_number;	/* Nominal scan number being displayed */
		JDIMENSION output_iMCU_row;	/* Number of iMCU rows read */


		int (*coef_bits)[DCTSIZE2];	/* -1 or current Al value for each coef */


		JQUANT_TBL * quant_tbl_ptrs[NUM_QUANT_TBLS];
		/* ptrs to coefficient quantization tables, or NULL if not defined */

		JHUFF_TBL * dc_huff_tbl_ptrs[NUM_HUFF_TBLS];
		JHUFF_TBL * ac_huff_tbl_ptrs[NUM_HUFF_TBLS];

		int data_precision;		/* bits of precision in image data */

		jpeg_component_info * comp_info;

		boolean progressive_mode;	/* TRUE if SOFn specifies progressive mode */
		boolean arith_code;		/* TRUE=arithmetic coding, FALSE=Huffman */

		UINT8 arith_dc_L[NUM_ARITH_TBLS]; /* L values for DC arith-coding tables */
		UINT8 arith_dc_U[NUM_ARITH_TBLS]; /* U values for DC arith-coding tables */
		UINT8 arith_ac_K[NUM_ARITH_TBLS]; /* Kx values for AC arith-coding tables */

		unsigned int restart_interval; /* MCUs per restart interval, or 0 for no restart */

		/* These fields record data obtained from optional markers recognized by
		* the JPEG library.
		*/
		boolean saw_JFIF_marker;	/* TRUE iff a JFIF APP0 marker was found */
		/* Data copied from JFIF marker; only valid if saw_JFIF_marker is TRUE: */
		UINT8 JFIF_major_version;	/* JFIF version number */
		UINT8 JFIF_minor_version;
		UINT8 density_unit;		/* JFIF code for pixel size units */
		UINT16 X_density;		/* Horizontal pixel density */
		UINT16 Y_density;		/* Vertical pixel density */
		boolean saw_Adobe_marker;	/* TRUE iff an Adobe APP14 marker was found */
		UINT8 Adobe_transform;	/* Color transform code from Adobe marker */

		boolean CCIR601_sampling;	/* TRUE=first samples are cosited */

		/* Aside from the specific data retained from APPn markers known to the
		* library, the uninterpreted contents of any or all APPn and COM markers
		* can be saved in a list for examination by the application.
		*/
		jpeg_saved_marker_ptr marker_list; /* Head of list of saved markers */

		/* Remaining fields are known throughout decompressor, but generally
		* should not be touched by a surrounding application.
		*/

		/*
		* These fields are computed during decompression startup
		*/
		int max_h_samp_factor;	/* largest h_samp_factor */
		int max_v_samp_factor;	/* largest v_samp_factor */

		int min_DCT_h_scaled_size;	/* smallest DCT_h_scaled_size of any component */
		int min_DCT_v_scaled_size;	/* smallest DCT_v_scaled_size of any component */

		JDIMENSION total_iMCU_rows;	/* # of iMCU rows in image */


		JSAMPLE * sample_range_limit; /* table for fast range-limiting */


		int comps_in_scan;		/* # of JPEG components in this scan */
		jpeg_component_info * cur_comp_info[MAX_COMPS_IN_SCAN];
		/* *cur_comp_info[i] describes component that appears i'th in SOS */

		JDIMENSION MCUs_per_row;	/* # of MCUs across the image */
		JDIMENSION MCU_rows_in_scan;	/* # of MCU rows in the image */

		int blocks_in_MCU;		/* # of DCT blocks per MCU */
		int MCU_membership[D_MAX_BLOCKS_IN_MCU];
		/* MCU_membership[i] is index in cur_comp_info of component owning */
		/* i'th block in an MCU */

		int Ss, Se, Ah, Al;		/* progressive JPEG parameters for scan */

		int unread_marker;

		struct jpeg_decomp_master * master;
		struct jpeg_d_main_controller * main;
		struct jpeg_d_coef_controller * coef;
		struct jpeg_d_post_controller * post;
		struct jpeg_input_controller * inputctl;
		struct jpeg_marker_reader * marker;
		struct jpeg_entropy_decoder * entropy;
		struct jpeg_inverse_dct * idct;
		struct jpeg_upsampler * upsample;
		struct jpeg_color_deconverter * cconvert;
		struct jpeg_color_quantizer * cquantize;
	};





	/* Error handler object */

	struct jpeg_error_mgr {
		/* Error exit handler: does not return to caller */
		JMETHOD(void, error_exit, (j_common_ptr cinfo));
		/* Conditionally emit a trace or warning message */
		JMETHOD(void, emit_message, (j_common_ptr cinfo, int msg_level));
		/* Routine that actually outputs a trace or error message */
		JMETHOD(void, output_message, (j_common_ptr cinfo));
		/* Format a message string for the most recent JPEG error or message */
		JMETHOD(void, format_message, (j_common_ptr cinfo, char * buffer));
#define JMSG_LENGTH_MAX  200	/* recommended size of format_message buffer */
		/* Reset error state variables at start of a new image */
		JMETHOD(void, reset_error_mgr, (j_common_ptr cinfo));

		/* The message ID code and any parameters are saved here.
		* A message can have one string parameter or up to 8 int parameters.
		*/
		int msg_code;
#define JMSG_STR_PARM_MAX  80
		union {
			int i[8];
			char s[JMSG_STR_PARM_MAX];
		} msg_parm;

		/* Standard state variables for error facility */

		int trace_level;		/* max msg_level that will be displayed */


		long num_warnings;		/* number of corrupt-data warnings */


		const char * const * jpeg_message_table; /* Library errors */
		int last_jpeg_message;    /* Table contains strings 0..last_jpeg_message */

		const char * const * addon_message_table; /* Non-library errors */
		int first_addon_message;	/* code for first string in addon table */
		int last_addon_message;	/* code for last string in addon table */
	};


	/* Progress monitor object */

	struct jpeg_progress_mgr {
		JMETHOD(void, progress_monitor, (j_common_ptr cinfo));

		long pass_counter;		/* work units completed in this pass */
		long pass_limit;		/* total number of work units in this pass */
		int completed_passes;		/* passes completed so far */
		int total_passes;		/* total number of passes expected */
	};


	/* Data destination object for compression */

	struct jpeg_destination_mgr {
		JOCTET * next_output_byte;	/* => next byte to write in buffer */
		size_t free_in_buffer;	/* # of byte spaces remaining in buffer */

		JMETHOD(void, init_destination, (j_compress_ptr cinfo));
		JMETHOD(boolean, empty_output_buffer, (j_compress_ptr cinfo));
		JMETHOD(void, term_destination, (j_compress_ptr cinfo));
	};


	/* Data source object for decompression */

	struct jpeg_source_mgr {
		const JOCTET * next_input_byte; /* => next byte to read from buffer */
		size_t bytes_in_buffer;	/* # of bytes remaining in buffer */

		JMETHOD(void, init_source, (j_decompress_ptr cinfo));
		JMETHOD(boolean, fill_input_buffer, (j_decompress_ptr cinfo));
		JMETHOD(void, skip_input_data, (j_decompress_ptr cinfo, long num_bytes));
		JMETHOD(boolean, resync_to_restart, (j_decompress_ptr cinfo, int desired));
		JMETHOD(void, term_source, (j_decompress_ptr cinfo));
	};

#define JPOOL_PERMANENT	0	/* lasts until master record is destroyed */
#define JPOOL_IMAGE	1	/* lasts until done with image/datastream */
#define JPOOL_NUMPOOLS	2

	typedef struct jvirt_sarray_control * jvirt_sarray_ptr;
	typedef struct jvirt_barray_control * jvirt_barray_ptr;


	struct jpeg_memory_mgr {
		/* Method pointers */
		JMETHOD(void *, alloc_small, (j_common_ptr cinfo, int pool_id,
			size_t sizeofobject));
		JMETHOD(void FAR *, alloc_large, (j_common_ptr cinfo, int pool_id,
			size_t sizeofobject));
		JMETHOD(JSAMPARRAY, alloc_sarray, (j_common_ptr cinfo, int pool_id,
			JDIMENSION samplesperrow,
			JDIMENSION numrows));
		JMETHOD(JBLOCKARRAY, alloc_barray, (j_common_ptr cinfo, int pool_id,
			JDIMENSION blocksperrow,
			JDIMENSION numrows));
		JMETHOD(jvirt_sarray_ptr, request_virt_sarray, (j_common_ptr cinfo,
			int pool_id,
			boolean pre_zero,
			JDIMENSION samplesperrow,
			JDIMENSION numrows,
			JDIMENSION maxaccess));
		JMETHOD(jvirt_barray_ptr, request_virt_barray, (j_common_ptr cinfo,
			int pool_id,
			boolean pre_zero,
			JDIMENSION blocksperrow,
			JDIMENSION numrows,
			JDIMENSION maxaccess));
		JMETHOD(void, realize_virt_arrays, (j_common_ptr cinfo));
		JMETHOD(JSAMPARRAY, access_virt_sarray, (j_common_ptr cinfo,
			jvirt_sarray_ptr ptr,
			JDIMENSION start_row,
			JDIMENSION num_rows,
			boolean writable));
		JMETHOD(JBLOCKARRAY, access_virt_barray, (j_common_ptr cinfo,
			jvirt_barray_ptr ptr,
			JDIMENSION start_row,
			JDIMENSION num_rows,
			boolean writable));
		JMETHOD(void, free_pool, (j_common_ptr cinfo, int pool_id));
		JMETHOD(void, self_destruct, (j_common_ptr cinfo));

		long max_memory_to_use;

		/* Maximum allocation request accepted by alloc_large. */
		long max_alloc_chunk;
	};

	typedef JMETHOD(boolean, jpeg_marker_parser_method, (j_decompress_ptr cinfo));

#ifdef HAVE_PROTOTYPES
#define JPP(arglist)	arglist
#else
#define JPP(arglist)	()
#endif

	/* Default error-management setup */
	EXTERN(struct jpeg_error_mgr *) jpeg_std_error
		JPP((struct jpeg_error_mgr * err));

#define jpeg_create_compress(cinfo) \
	jpeg_CreateCompress((cinfo),  \
	(size_t) sizeof(struct jpeg_compress_struct))
#define jpeg_create_decompress(cinfo) \
	jpeg_CreateDecompress((cinfo), \
	(size_t) sizeof(struct jpeg_decompress_struct))
	EXTERN(void) jpeg_CreateCompress JPP((j_compress_ptr cinfo,
		size_t structsize));
	EXTERN(void) jpeg_CreateDecompress JPP((j_decompress_ptr cinfo,
		size_t structsize));
	/* Destruction of JPEG compression objects */
	EXTERN(void) jpeg_destroy_compress JPP((j_compress_ptr cinfo));
	EXTERN(void) jpeg_destroy_decompress JPP((j_decompress_ptr cinfo));

	/* Standard data source and destination managers: stdio streams. */
	/* Caller is responsible for opening the file before and closing after. */
	EXTERN(void) jpeg_stdio_dest JPP((j_compress_ptr cinfo, FILE * outfile));
	EXTERN(void) jpeg_stdio_src JPP((j_decompress_ptr cinfo, FILE * infile));

	/* Default parameter setup for compression */
	EXTERN(void) jpeg_set_defaults JPP((j_compress_ptr cinfo));
	/* Compression parameter setup aids */
	EXTERN(void) jpeg_set_colorspace JPP((j_compress_ptr cinfo,
		J_COLOR_SPACE colorspace));
	EXTERN(void) jpeg_default_colorspace JPP((j_compress_ptr cinfo));
	EXTERN(void) jpeg_set_quality JPP((j_compress_ptr cinfo, int quality,
		boolean force_baseline));
	EXTERN(void) jpeg_set_linear_quality JPP((j_compress_ptr cinfo,
		int scale_factor,
		boolean force_baseline));
	EXTERN(void) jpeg_default_qtables JPP((j_compress_ptr cinfo,
		boolean force_baseline));
	EXTERN(void) jpeg_add_quant_table JPP((j_compress_ptr cinfo, int which_tbl,
		const unsigned int *basic_table,
		int scale_factor,
		boolean force_baseline));
	EXTERN(int) jpeg_quality_scaling JPP((int quality));
	EXTERN(void) jpeg_simple_progression JPP((j_compress_ptr cinfo));
	EXTERN(void) jpeg_suppress_tables JPP((j_compress_ptr cinfo,
		boolean suppress));
	EXTERN(JQUANT_TBL *) jpeg_alloc_quant_table JPP((j_common_ptr cinfo));
	EXTERN(JHUFF_TBL *) jpeg_alloc_huff_table JPP((j_common_ptr cinfo));

	/* Main entry points for compression */
	EXTERN(void) jpeg_start_compress JPP((j_compress_ptr cinfo,
		boolean write_all_tables));
	EXTERN(JDIMENSION) jpeg_write_scanlines JPP((j_compress_ptr cinfo,
		JSAMPARRAY scanlines,
		JDIMENSION num_lines));
	EXTERN(void) jpeg_finish_compress JPP((j_compress_ptr cinfo));

	/* Precalculate JPEG dimensions for current compression parameters. */
	EXTERN(void) jpeg_calc_jpeg_dimensions JPP((j_compress_ptr cinfo));

	/* Replaces jpeg_write_scanlines when writing raw downsampled data. */
	EXTERN(JDIMENSION) jpeg_write_raw_data JPP((j_compress_ptr cinfo,
		JSAMPIMAGE data,
		JDIMENSION num_lines));

	/* Write a special marker.  See libjpeg.txt concerning safe usage. */
	EXTERN(void) jpeg_write_marker
		JPP((j_compress_ptr cinfo, int marker,
		const JOCTET * dataptr, unsigned int datalen));
	/* Same, but piecemeal. */
	EXTERN(void) jpeg_write_m_header
		JPP((j_compress_ptr cinfo, int marker, unsigned int datalen));
	EXTERN(void) jpeg_write_m_byte
		JPP((j_compress_ptr cinfo, int val));

	/* Alternate compression function: just write an abbreviated table file */
	EXTERN(void) jpeg_write_tables JPP((j_compress_ptr cinfo));

	/* Decompression startup: read start of JPEG datastream to see what's there */
	EXTERN(int) jpeg_read_header JPP((j_decompress_ptr cinfo,
		boolean require_image));
	/* Return value is one of: */
#define JPEG_SUSPENDED		0 /* Suspended due to lack of input data */
#define JPEG_HEADER_OK		1 /* Found valid image datastream */
#define JPEG_HEADER_TABLES_ONLY	2 /* Found valid table-specs-only datastream */


	/* Main entry points for decompression */
	EXTERN(boolean) jpeg_start_decompress JPP((j_decompress_ptr cinfo));
	EXTERN(JDIMENSION) jpeg_read_scanlines JPP((j_decompress_ptr cinfo,
		JSAMPARRAY scanlines,
		JDIMENSION max_lines));
	EXTERN(boolean) jpeg_finish_decompress JPP((j_decompress_ptr cinfo));

	/* Replaces jpeg_read_scanlines when reading raw downsampled data. */
	EXTERN(JDIMENSION) jpeg_read_raw_data JPP((j_decompress_ptr cinfo,
		JSAMPIMAGE data,
		JDIMENSION max_lines));

	/* Additional entry points for buffered-image mode. */
	EXTERN(boolean) jpeg_has_multiple_scans JPP((j_decompress_ptr cinfo));
	EXTERN(boolean) jpeg_start_output JPP((j_decompress_ptr cinfo,
		int scan_number));
	EXTERN(boolean) jpeg_finish_output JPP((j_decompress_ptr cinfo));
	EXTERN(boolean) jpeg_input_complete JPP((j_decompress_ptr cinfo));
	EXTERN(void) jpeg_new_colormap JPP((j_decompress_ptr cinfo));
	EXTERN(int) jpeg_consume_input JPP((j_decompress_ptr cinfo));
	/* Return value is one of: */
	/* #define JPEG_SUSPENDED	0    Suspended due to lack of input data */
#define JPEG_REACHED_SOS	1 /* Reached start of new scan */
#define JPEG_REACHED_EOI	2 /* Reached end of image */
#define JPEG_ROW_COMPLETED	3 /* Completed one iMCU row */
#define JPEG_SCAN_COMPLETED	4 /* Completed last iMCU row of a scan */

	/* Precalculate output dimensions for current decompression parameters. */
	EXTERN(void) jpeg_calc_output_dimensions JPP((j_decompress_ptr cinfo));

	/* Control saving of COM and APPn markers into marker_list. */
	EXTERN(void) jpeg_save_markers
		JPP((j_decompress_ptr cinfo, int marker_code,
		unsigned int length_limit));

	/* Install a special processing method for COM or APPn markers. */
	EXTERN(void) jpeg_set_marker_processor
		JPP((j_decompress_ptr cinfo, int marker_code,
		jpeg_marker_parser_method routine));

	/* Read or write raw DCT coefficients --- useful for lossless transcoding. */
	EXTERN(jvirt_barray_ptr *) jpeg_read_coefficients JPP((j_decompress_ptr cinfo));
	EXTERN(void) jpeg_write_coefficients JPP((j_compress_ptr cinfo,
		jvirt_barray_ptr * coef_arrays));
	EXTERN(void) jpeg_copy_critical_parameters JPP((j_decompress_ptr srcinfo,
		j_compress_ptr dstinfo));

	EXTERN(void) jpeg_abort_compress JPP((j_compress_ptr cinfo));
	EXTERN(void) jpeg_abort_decompress JPP((j_decompress_ptr cinfo));

	EXTERN(void) jpeg_abort JPP((j_common_ptr cinfo));
	EXTERN(void) jpeg_destroy JPP((j_common_ptr cinfo));

	EXTERN(boolean) jpeg_resync_to_restart JPP((j_decompress_ptr cinfo,
		int desired));


#define JPEG_RST0	0xD0	/* RST0 marker code */
#define JPEG_EOI	0xD9	/* EOI marker code */
#define JPEG_APP0	0xE0	/* APP0 marker code */
#define JPEG_COM	0xFE	/* COM marker code */

#ifdef JPEG_INTERNALS
#include "jpegint.h"		/* fetch private declarations */
#include "jerror.h"		/* fetch error codes too */
#endif

#ifdef __cplusplus
#ifndef DONT_USE_EXTERN_C
}
#endif
#endif

#endif /* JPEGLIB_H */
