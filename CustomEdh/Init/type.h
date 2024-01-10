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

typedef enum  {
	IC27,	// Vdet_1, Vg_att_2,  Vg_att_1,  Vg_PA_1,  VgPA_2
	IC28,	//VDET_SYNT_3, Id_PA_1, Idx2_mult_1, Idx3_mult_1, Id_amp_mult_1, Vgx2_mult_1, Vgx3_mult_1, Vgamp_mult_1
	IC31	//Vdet_2, Id_PA_2, Idx2_mult_2, Idx3_mult_2, Id_amp_mult_2, Vgx2_mult_2, Vgx3_mult_2, Vgamp_mult_2
}dac_t;

typedef enum {
	set_f=0,		//setta la freuenza
	on_f,			//abilita sint
	off_f,			//disabilita sint
	set_f1_max,		//in caso di sweep questo è il valore max di f1, il valore min è impostato con set_f1
	set_delta_f,	//con due toni questo fissa il delta
	set_sweep_time,	//setta lo sweep time, se servirà.
	set_mode,		//imposta la modalità operativa
	read_sint,		//legge i dati in ram del sint
	write_reg		//scrive i registri singoli dei sint
} sint_op_t ;

typedef enum {
	indep,							//tre toni tutti indipendenti
	tone1_track_tone2,				//due TONI RF agganciati in sweep
	sweepsingle						//tono uno in sweep
} sint_mode_t ;

typedef enum {
	remote,			//messaggio scritto in coda da usb o eth
	gui				//messaggio scritto in coda da gui
} source_t ;

//l'idea è che l'unico sweep utile sia quello del singolo tono, supponiamo la catena numero 1. In questo caso i limiti di sweep saranno i parametri freq1 e f1_sweep_max
//quando messo in tracking freq1 indica il tono 1 mentre freq2 indicherà il delta del tono 2 dal tono 1. Vedere come far sweeppare il delta.
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

struct sint_msg_t{
	sint_mode_t mode;
	uint64_t value;
	sint_op_t op;
	uint16_t addr;
	uint8_t data;
	uint8_t sint;
	source_t source;
};
// tutto da rivedere.......
struct sint_fram_parameters{
	uint64_t start_marker;
	signal_t sint_par;
	uint64_t end_marker;
}__attribute__((__packed__));

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
	firm_closed_loop
};

struct bias
{
	uint8_t index;
	uint8_t op;
	uint8_t tipo;
	uint32_t valore;
	uint32_t min;
	uint32_t max;
};

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

struct bias_parameters
{
	uint64_t start_marker;
	struct bias polarizzazione[MAX_BIAS_INDEX];
	uint64_t end_marker;
}__attribute__((__packed__));

enum Sint_number
{
	Sint1=0,
	Sint2,
	Sint3
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


#endif /* INIT_TYPE_H_ */
