import { useEffect, useState } from "react";
import uploadimg from "../img/uploadsym.png";

export default function FileUpload() {
  const [files, setFiles] = useState([]);
  const [isUploaded, setIsUploaded] = useState(false);

  function upload() {
    let formdata = new FormData();
    for (let i = 0; i < files.length; i++) {
      formdata.append(`file${i}`, files[i]);
    }
  }
  function selectFiles(event) {
    const newFiles = Array.from(event.target.files);
    setFiles((prevFiles) => [...prevFiles, ...newFiles]);
    setIsUploaded(true);
  }
  function handleDrop(event) {
    event.preventDefault();
    const newFiles = Array.from(event.dataTransfer.files);
    const imageFiles = newFiles.filter((file) =>
      file.type.startsWith("image/")
    );
    if(imageFiles.length === 0) return;
    setFiles((prevFiles) => [...prevFiles, ...imageFiles]);
    setIsUploaded(true);
  }

  function deleteFile(filename){
    const updatedFiles = files.filter((file) =>
      file.name !== filename
    );
    setFiles(updatedFiles);
    if(updatedFiles.length === 0) setIsUploaded(false);
  }

  return (
    <div className="flex justify-center">
      <form
        className="w-3/4 h-72 rounded-2xl bg-indigo-950 md:w-2/4 lg:h-96 mt-10 mb-10 flex flex-col justify-center border-blue-800 border-2"
        onDrop={handleDrop}
        onDragOver={(event) => event.preventDefault()}
      >
        <input
          type="file"
          hidden
          id="browse"
          onChange={selectFiles}
          accept=".png, .jpg, .jpeg"
          multiple
        />
        {!isUploaded && (
          <div>
            <div>
              <div className="flex justify-center">
                <img src={uploadimg} alt="" className="w-10 lg:w-24"></img>
              </div>
              <p className="text-center mt-4 text-white">
                Drag and Drop Images
              </p>
              <p className="text-center mt-4 text-white">Or</p>
            </div>
            <div className="flex justify-center">
              <label
                className="text-center mt-4 cursor-pointer bg-white pt-2 pb-2 pr-4 pl-4 rounded-md font-semibold"
                htmlFor="browse"
              >
                Browse Files
              </label>
            </div>
          </div>
        )}
        {isUploaded && (
          <>
            <div className="flex flex-col overflow-y-auto h-4/5 m-1">
              {files.map((file, index) => (
                <div
                  key={index}
                  className="bg-white border border-gray-300 rounded-md shadow-lg p-2 flex m-1 justify-between"
                >
                  <h2 className="text-lg font-semibold">{file.name}</h2>
                  <div onClick={() => deleteFile(file.name)}>X</div>
                </div>
              ))}
            </div>
            <div className="h-1/5 flex justify-center items-center">
              <div className="w-40 flex justify-evenly">
                <label
                  className="text-center cursor-pointer bg-white rounded-md font-semibold pt-2 pb-2"
                  htmlFor="browse"
                >
                  Add More
                </label>
                <label className="text-center cursor-pointer bg-white rounded-md font-semibold pt-2 pb-2">
                  Stitch
                </label>
              </div>
            </div>
          </>
        )}
      </form>
    </div>
  );
}
