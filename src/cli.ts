import {
  clonefile,
  CLONE_NOFOLLOW,
  CLONE_NOOWNERCOPY,
  CLONE_ACL,
} from './index';
process.env.NO_COLOR = '1';
const FLAGS = {
  '--acl': CLONE_ACL,
  '--nofollow': CLONE_NOFOLLOW,
  '--noownercopy': CLONE_NOOWNERCOPY,
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
let flags: number = 0;
process.argv.forEach((arg) => {
  if (arg in FLAGS) {
    flags |= FLAGS[arg];
  }
});

clonefile(src, dest, flags, (err?: any) => {
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
