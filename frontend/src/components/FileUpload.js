import { useEffect, useState } from "react";
import uploadimg from "../img/uploadsym.png";

export default function FileUpload() {
  const [files, setFiles] = useState([]);
  const [isUploaded, setIsUploaded] = useState(false);
  const [image, setImage] = useState(null);

  function upload() {
    let formdata = new FormData();
    for (let i = 0; i < files.length; i++) {
      formdata.append("files", files[i]);
    }
    fetch("http://localhost:8080/api/stitch-images", {
      method: "POST",
      body: formdata
    })
    .then((response) => {
      if (!response.ok) {
        throw new Error(`HTTP error! Status: ${response.status}`);
      }
      return response.blob();
    })
    .then((blob) => {
      const imageUrl = URL.createObjectURL(blob);
      setImage(imageUrl);
    })
    .catch((error) => {
      console.error("Upload error:", error);
      alert("Failed to upload files.");
    })
  }
  function selectFiles(event) {
    const newFiles = Array.from(event.target.files);
    setFiles((prevFiles) => [...prevFiles, ...newFiles]);
    if (newFiles.length === 0) return;
    setIsUploaded(true);
  }
  function handleDrop(event) {
    event.preventDefault();
    const newFiles = Array.from(event.dataTransfer.files);
    const imageFiles = newFiles.filter((file) =>
      file.type.startsWith("image/")
    );
    if (imageFiles.length === 0) return;
    setFiles((prevFiles) => [...prevFiles, ...imageFiles]);
    setIsUploaded(true);
  }

  function deleteFile(filename, event) {
    event.preventDefault();
    const updatedFiles = files.filter((file) => file.name !== filename);
    setFiles(updatedFiles);
    if (updatedFiles.length === 0) setIsUploaded(false);
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
                  className="border border-none bg-white bg-opacity-10 rounded-md shadow-lg p-2 flex m-1 mr-2 ml-2 justify-between"
                >
                  <h2 className="text-lg font-semibold text-white">
                    {file.name}
                  </h2>
                  <button
                    className="cursor-pointer text-white"
                    onClick={(event) => deleteFile(file.name, event)}
                  >
                    X
                  </button>
                </div>
              ))}
            </div>
            <div className="h-1/5 flex justify-center items-center">
              <div className="w-60 flex justify-evenly">
                <label
                  className="text-center cursor-pointer bg-white rounded-md font-semibold p-2"
                  htmlFor="browse"
                >
                  Add More
                </label>
                <button onClick={upload}
                className="text-center cursor-pointer bg-white rounded-md font-semibold p-2 hover:bg-white hover:text-purple-600">
                  
                  Stitch
                </button>
              </div>
            </div>
          </>
        )}
      </form>

      {/* Display Processed Image */}
      {image && (
        <div className="mt-5">
          <h3 className="text-white text-lg">Procesed Image</h3>
          <img src={image} alt="Processed Output" className="mt-2 border-2 border-white rounded-lg" />
        </div>
      )}
    </div>
  );
}
