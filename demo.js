const anb = require('./build/Release/anb');

console.log("ANB = ", anb);

const project = new anb.Project(1, "./demo_project");
console.log("project = ", project);
console.log("project.getValue() = ", project.getValue());
console.log("project.getPath() = ", project.getPath());

//const project = anb.createProject("./demo_project");
//console.log("project.path = "+project.path)

const file = project.createFile("tofu");
console.log("file = project.getFile('tofu') = ", file);

const entity = file.createEntity();
console.log("entity = file.createEntity() = ", entity);



