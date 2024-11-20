type Callback = (err?: Error) => void;
export declare function clonefile(
  src: string,
  dest: string,
  flags: number | Callback,
  cb?: Callback
): any;
export declare const CLONE_NOFOLLOW = 1;
export declare const CLONE_NOOWNERCOPY = 2;
export declare const CLONE_ACL = 4;
declare const _default: {
  clonefile: typeof clonefile;
  CLONE_NOFOLLOW: number;
  CLONE_NOOWNERCOPY: number;
  CLONE_ACL: number;
};
export default _default;