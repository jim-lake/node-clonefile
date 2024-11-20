'use strict';
Object.defineProperty(exports, '__esModule', { value: true });
const index_1 = require('./index');
process.env.NO_COLOR = '1';
const FLAGS = {
  '--acl': index_1.CLONE_ACL,
  '--nofollow': index_1.CLONE_NOFOLLOW,
  '--noownercopy': index_1.CLONE_NOOWNERCOPY,
};
const FLAG_S = Object.keys(FLAGS)
  .map((flag) => `[${flag}]`)
  .join(' ');
if (process.argv.length < 3) {
  usage();
}
if (process.argv.find((arg) => arg === '-h' || arg === '-?')) {
  usage();
}
const dest = process.argv.pop();
const src = process.argv.pop();
if (FLAGS[src] || FLAGS[dest]) {
  usage();
}
let flags = 0;
process.argv.forEach((arg) => {
  if (arg in FLAGS) {
    flags |= FLAGS[arg] ?? 0;
  }
});
(0, index_1.clonefile)(src, dest, flags, (err) => {
  if (err) {
    console.error('clonefile: error:', err.message);
    process.exit(err.errno || -2);
  } else {
    process.exit(0);
  }
});
function usage() {
  console.log(`clonefile ${FLAG_S} <src> <dest>`);
  process.exit(-1);
}
//# sourceMappingURL=cli.js.map
