'use strict';
Object.defineProperty(exports, '__esModule', { value: true });
exports.CLONE_OVERWRITE =
  exports.CLONE_ACL =
  exports.CLONE_NOOWNERCOPY =
  exports.CLONE_NOFOLLOW =
    void 0;
exports.clonefile = clonefile;
const addon = require('../build/Release/node_clonefile.node');
function clonefile(src, dest, flags, cb) {
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
exports.CLONE_NOFOLLOW = 0x01;
exports.CLONE_NOOWNERCOPY = 0x02;
exports.CLONE_ACL = 0x04;
exports.CLONE_OVERWRITE = 0x10000;
exports.default = {
  clonefile,
  CLONE_NOFOLLOW: exports.CLONE_NOFOLLOW,
  CLONE_NOOWNERCOPY: exports.CLONE_NOOWNERCOPY,
  CLONE_ACL: exports.CLONE_ACL,
  CLONE_OVERWRITE: exports.CLONE_OVERWRITE,
};
//# sourceMappingURL=index.js.map
