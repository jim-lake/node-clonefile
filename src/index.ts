const addon = require('../build/Release/node_clonefile.node');

type Callback = (err?: Error) => void;

export function clonefile(
  src: string,
  dest: string,
  flags: number | Callback,
  cb?: Callback
) {
  if (typeof flags === 'function') {
    cb = flags;
    flags = 0;
  }
  const ret = addon.clonefile(src, dest, flags, cb);
  if (ret && typeof cb === 'function') {
    cb(new Error(ret));
  }
  return ret;
}
export const CLONE_NOFOLLOW = 0x01;
export const CLONE_NOOWNERCOPY = 0x02;
export const CLONE_ACL = 0x04;
export default {
  clonefile,
  CLONE_NOFOLLOW,
  CLONE_NOOWNERCOPY,
  CLONE_ACL,
};
