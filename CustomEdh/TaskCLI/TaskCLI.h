/*
 * TaskCLI.h
 *
 *  Created on: Jul 16, 2023
 *      Author: giuli
 */

#ifndef TASKCLI_TASKCLI_H_
#define TASKCLI_TASKCLI_H_


#include "embedded_cli.h"

#define CLIStackSize 1024*4

#define CLI_BUFFER_SIZE 164
#define CLI_RX_BUFFER_SIZE 16
#define CLI_CMD_BUFFER_SIZE 64
#define CLI_HISTORY_SIZE 32
#define CLI_BINDING_COUNT 22

void InitCLITask(void);

void InitCLITask(void);

void writeChar(EmbeddedCli *embeddedCli, char c);

void onCommand(EmbeddedCli *embeddedCli, CliCommand *command);

//void writeChar(EmbeddedCli *embeddedCli, char c);

void onHello(EmbeddedCli *cli, char *args, void *context);

void onLed(EmbeddedCli *cli, char *args, void *context);

void onAdc(EmbeddedCli *cli, char *args, void *context);

void onTest(EmbeddedCli *cli, char *args, void *context);

void onBiasWrite(EmbeddedCli *cli, char *args, void *context);

void onBiasRead(EmbeddedCli *cli, char *args, void *context);

void onDumpFram(EmbeddedCli *cli, char *args, void *context);

void onWriteFram(EmbeddedCli *cli, char *args, void *context);

void onWritePwrCtrl(EmbeddedCli *cli, char *args, void *context);

void onReadPwrCtrl(EmbeddedCli *cli, char *args, void *context);

void onSetPwr(EmbeddedCli *cli, char *args, void *context);

void onSavePwrTable(EmbeddedCli *cli, char *args, void *context);

void onLoadTable(EmbeddedCli *cli, char *args, void *context);

void onSaveBias(EmbeddedCli *cli, char *args, void *context);

void onLoadSint(EmbeddedCli *cli, char *args, void *context);

void onSaveSint(EmbeddedCli *cli, char *args, void *context);

void onSetSint(EmbeddedCli *cli, char *args, void *context);

void onReadSint(EmbeddedCli *cli, char *args, void *context);

void onDumpsdram(EmbeddedCli *cli, char *args, void *context);

void onReadVersion(EmbeddedCli *cli, char *args, void *context);

#endif /* TASKCLI_TASKCLI_H_ */
