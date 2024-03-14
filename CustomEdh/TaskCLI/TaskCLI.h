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
#define CLI_BINDING_COUNT 11

void InitCLITask(void);

void InitCLITask(void);

bool isHexadecimal(const char *s);

bool isdecimal(const char *s);

void writeChar(EmbeddedCli *embeddedCli, char c);

void onCommand(EmbeddedCli *embeddedCli, CliCommand *command);

//void writeChar(EmbeddedCli *embeddedCli, char c);

void onHello(EmbeddedCli *cli, char *args, void *context);

void onLed(EmbeddedCli *cli, char *args, void *context);

void onAdc(EmbeddedCli *cli, char *args, void *context);

void onTest(EmbeddedCli *cli, char *args, void *context);

void onDumpFram(EmbeddedCli *cli, char *args, void *context);

void onWriteFram(EmbeddedCli *cli, char *args, void *context);

void onWritePwrCtrl(EmbeddedCli *cli, char *args, void *context);

void onReadPwrCtrl(EmbeddedCli *cli, char *args, void *context);

void onSavePwrTable(EmbeddedCli *cli, char *args, void *context);

void onLoadTable(EmbeddedCli *cli, char *args, void *context);

void onDumpsdram(EmbeddedCli *cli, char *args, void *context);

void onReadVersion(EmbeddedCli *cli, char *args, void *context);

void onCmd(EmbeddedCli *cli, char *args, void *context);

void onEnDrain(EmbeddedCli *cli, char *args, void *context);

void onWritesdram(EmbeddedCli *cli, char *args, void *context);

#endif /* TASKCLI_TASKCLI_H_ */
