/*
 * type.h
 *
 *  Created on: 8 lug 2023
 *      Author: giuli
 */

#ifndef INIT_TYPE_H_
#define INIT_TYPE_H_

#define DAC_0 0
#define DAC_1 1
#define DAC_2 2
#define DAC_3 3
#define DAC_4 4
#define DAC_5 5
#define DAC_6 6
#define DAC_7 7



#define MAX_BIAS_INDEX 11
#define MAX_PWR_TABLE_SIZE 41
#define MAX_FREQ_CALIB 16
#define MAX_FLAT_POWER 10


#define NUM_PWRCTRL 3

#define BIAS_FRAM_ADDR 0
#define PARAM_FRAM_ADDR 0

#define MAX_SWEEP_STEP 1e9
#define MAX_DELTA_TONE 1e9
#define MAX_SWEEP_POWER_STEP 20

typedef enum {
	s_OFF,
	s_ON
}on_off_t;

typedef enum  {
	IC27,	// Vdet_1, Vg_att_2,  Vg_att_1,  Vg_PA_1,  VgPA_2
	IC28,	//VDET_SYNT_3, Id_PA_1, Idx2_mult_1, Idx3_mult_1, Id_amp_mult_1, Vgx2_mult_1, Vgx3_mult_1, Vgamp_mult_1
	IC31	//Vdet_2, Id_PA_2, Idx2_mult_2, Idx3_mult_2, Id_amp_mult_2, Vgx2_mult_2, Vgx3_mult_2, Vgamp_mult_2
}dac_t;


typedef enum {
	indep,							//tre toni tutti indipendenti
	tone1_track_tone2,				//due TONI RF agganciati in sweep
	sweepsingle,						//tono uno in sweep
	sweeppower,
	end_sint_mode									//serve solo per fare un check se un valore apparterrà a questo enum, deve sempre essere l'ultimo
} sint_mode_t ;

typedef enum {
	remote,			//messaggio scritto in coda da usb o eth
	gui				//messaggio scritto in coda da gui
} source_t ;

// da cancellare
typedef struct {
	uint64_t freq_1;
	uint64_t freq_2;
	uint64_t freq_3;
	uint64_t deltatone;
	uint64_t f1_sweep_max;
	uint64_t sweep_time;
	uint8_t sint1_on_off;
	uint8_t sint2_on_off;
	uint8_t sint3_on_off;
	sint_mode_t modo;
} signal_t;


enum operation
{
	write=0,
	read,
	setpower	//è come il write ma non modifica i limiti dei polarizzatori
};

enum biastype
{
	forward=0,
	hard_closed_loop,
	firm_closed_loop,
	end_bias_type
};

enum Sint_number
{
	Sint1=0,
	Sint2,
	Sint3,
	end_Sint
};

enum {
	ADF4371_CH_RF8,
	ADF4371_CH_RFAUX8,
	ADF4371_CH_RF16
};

struct adf4371_dev {
	uint32_t	ref_div_factor;
	uint32_t	clkin_freq;
	uint32_t	fpfd;
	uint32_t	integer;
	uint32_t	fract1;
	uint32_t	fract2;
	uint32_t	mod2;
	uint32_t	rf_div_sel;
    uint32_t    channel;
	uint32_t	cp_current;
};

enum {
	data=0,
	pmin,
	pmax,
	threshold,
	fminima,
	fmassima
};

struct fwd_pwr_ctrl {
	uint16_t ctrl[MAX_PWR_TABLE_SIZE][MAX_FREQ_CALIB];
	double pmax;
	double threshold;
	double pmin;
	double fmin;
	double fmax;
};

struct fwd_ram_pwr_ctrl {
	uint64_t start_marker;
	struct fwd_pwr_ctrl fwd_pwr_ctrl[NUM_PWRCTRL];	// indice la catena
	uint64_t end_marker;
}__attribute__((__packed__));


typedef enum {
	set_freq,
	sint_en,
	set_fmin,
	set_fmax,
	set_delta_tone,
	set_swp_time,
	set_freq_mode,
	get_struct,
	set_sint_reg,
	set_pol_value,
	set_type_min_max_pol,
	get_pol_struct,
	set_power,
	set_pmin,
	set_pmax,
	set_pow_mode,
	get_struct_power,
	save_parameters,
	load_parameters,
	set_sweep_power
} opto_code_t ;

struct msg_t{
	opto_code_t op;
	double par1;
	double par2;
	double par3;
	double par4;
	source_t source;
};


typedef enum {
	no_control,					//nessuna variazione della polarizzazione, si usa il valore settatosui polarizzatori settato manualmente con comando
	auto_control,				//la polarizzazione varia a seconda della potenza impostata e dalla frequenza
	end_power_mode							//serve solo per fare un check se un valore apparterrà a questo enum, deve sempre essere l'ultimo
} power_mode_t ;


//	index 0 -> bias_PA_1;
//	index 1 -> bias_PA_2;
//	index 2 -> x2_1;
//	index 3 -> x3_1;
//	index 4 -> amp_mult_1;
//	index 5 -> x2_2;
//	index 6 -> x3_2;
//	index 7 -> amp_mult_2;
//	index 8 -> att_1;
//	index 9 -> att_2;
//  index 10 -> attenuatore variabile della catena di OL non moltiplicata
typedef struct
{
	uint8_t index;
	uint8_t op;
	uint8_t tipo;
	uint32_t valore;
	uint32_t min;
	uint32_t max;
} bias_power_t;

typedef struct
{
	double power[3];
	double sweep_pmin;
	double sweep_pmax;
	double power_sweep_step;
	power_mode_t p_mode;
} par_instant_t;

typedef struct {
	uint64_t freq[3];
	uint64_t deltatone;
	uint64_t f1_sweep_min;
	uint64_t f1_sweep_max;
	uint64_t sweep_step;
	uint8_t sint_on_off[3];
	sint_mode_t modo;
} sint_t;

struct parameters
{
	uint64_t start_marker;
	bias_power_t polarizzazione[MAX_BIAS_INDEX];
	sint_t sint_par;
	par_instant_t settings;
	uint64_t end_marker;
}__attribute__((__packed__));

#endif /* INIT_TYPE_H_ */
