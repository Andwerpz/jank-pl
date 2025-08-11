import * as path from 'path';
import * as vscode from 'vscode';
import {
  LanguageClient,
  LanguageClientOptions,
  ServerOptions,
} from 'vscode-languageclient/node';

export function activate(context: vscode.ExtensionContext) {
  const serverExe = process.platform === 'win32' ? 'mylsp.exe' : './mylsp';
  const serverPath = context.asAbsolutePath(path.join('server', serverExe));

  const serverOptions: ServerOptions = {
    run: { command: serverPath },
    debug: { command: serverPath }
  };

  const clientOptions: LanguageClientOptions = {
    documentSelector: [{ scheme: 'file', language: 'mylang' }]
  };

  const client = new LanguageClient('mylang', 'My Language Server', serverOptions, clientOptions);
  context.subscriptions.push(client.start());
}
