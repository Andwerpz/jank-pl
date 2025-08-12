import {
	createConnection,
	TextDocuments,
	ProposedFeatures,
	InitializeParams,
	TextDocumentSyncKind,
	InitializeResult,
} from "vscode-languageserver/node";

import { spawn } from 'child_process';

import { TextDocument } from "vscode-languageserver-textdocument";

// Create a connection for the server, using Node's IPC as a transport.
// Also include all preview / proposed LSP features.
const connection = createConnection(ProposedFeatures.all);

// Create a simple text document manager.
const documents: TextDocuments<TextDocument> = new TextDocuments(TextDocument);

const tokenTypesLegend = [
	'type', 'string', 'keyword', 'number', 'comment'
]

connection.onInitialize((params: InitializeParams) => {
	const result: InitializeResult = {
		capabilities: {
			textDocumentSync: TextDocumentSyncKind.Incremental,
			semanticTokensProvider: {
				legend: {
					tokenTypes: tokenTypesLegend,
					tokenModifiers: []
				},
				full: true
			}
		},
	};
	return result;
});

connection.languages.semanticTokens.on(async (params) => {
	const document = documents.get(params.textDocument.uri);
	if (!document) return { data: [] };

	const text = document.getText();
	const tokens = await runParserCLI(text); 
	const data = encodeTokensLSP(tokens);

	return { data: data };
});

function runParserCLI(text: string): Promise<any[]> {
	return new Promise((resolve, reject) => {
		const parserPath = __dirname + "/parser_cli";
		const proc = spawn(parserPath);
		let output = '';

		proc.stdout.on('data', (chunk) => output += chunk);
		proc.stderr.on('data', (err) => console.error(err.toString()));

		proc.on('close', (code) => {
			if (code !== 0) reject(new Error(`CLI exited with code ${code}`));
			else resolve(JSON.parse(output));
		});

		proc.stdin.write(text);
		proc.stdin.end();
	});
}

function encodeTokensLSP(tokens: any[]): number[] {
	const data: number[] = [];
	let prev_line = 0;
	let prev_col = 0;

	for (const t of tokens) {
		console.log("ENCODING TOKEN : " + JSON.stringify(t));

		const delta_line = t.line - prev_line;
		const delta_col = delta_line === 0 ? t.col - prev_col : t.col;
		prev_line = t.line;
		prev_col = t.col;

		const tokenTypeIndex = tokenTypesLegend.indexOf(t.type);
		const tokenModifierBitset = 0;

		data.push(delta_line, delta_col, t.length, tokenTypeIndex, tokenModifierBitset);
	}

	return data;
}

// Make the text document manager listen on the connection
// for open, change and close text document events
documents.listen(connection);

// Listen on the connection
connection.listen();
