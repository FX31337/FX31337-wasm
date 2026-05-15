export type LibModule = Record<string, any>;
export type ContentJsLoader = (options: {
    locateFile: (path: string) => string;
}) => Promise<LibModule>;
export declare class Test {
    run(lib: LibModule): void;
}
export declare function runTest(what: new () => Test, contentJs?: string | ContentJsLoader, contentWasm?: string): Promise<void>;
export declare function run(what: new () => Test, contentJs?: string | ContentJsLoader, contentWasm?: string): void;
