const M = require('./build/Release/node_nan_minimal');

console.log("M = ", M);

const a = new M.A();
console.log("a = ",a);

const b = a.foo();
console.log("b = ",b);


const a2 = M.A();
console.log("a2 = ",a2);

