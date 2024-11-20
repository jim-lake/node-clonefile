const { clonefile } = require('../dist/index');

clonefile('test1.bin', 'test2.bin', (err) => {
  console.log('err:', err);
});

clonefile('test3.bin', 'test4.bin', 0, (err2) => {
  console.log('err2:', err2);
});
