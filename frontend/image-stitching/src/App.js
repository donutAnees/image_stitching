import FileUpload from "./components/FileUpload";
import Hero from "./components/Hero";

function App() {
  return (
    <div>
      <Hero>
        <h1 className="text-center text-6xl text-white font-semibold pt-10 pr-4 pl-4">
          Stitch images <span className="text-transparent bg-clip-text bg-gradient-to-r from-blue-600 to-purple-500">Seamlessly</span>.
        </h1>
        <FileUpload />
      </Hero>
    </div>
  );
}

export default App;
