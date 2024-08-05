import { useState } from "react";

export default function FileUpload() {
  const [files, setFiles] = useState([]);
  function upload() {
    let formdata = new FormData();
    for (let i = 0; i < files.length; i++) {
      formdata.append(`file${i}`, files[i]);
    }
  }
  function selectFiles(event) {
    const newFiles = Array.from(event.target.files);
    setFiles((prevFiles) => [...prevFiles, ...newFiles]);
  }
  function handleDrop(event) {
    event.preventDefault();
    const newFiles = Array.from(event.dataTransfer.files);
    const imageFiles = newFiles.filter(file => file.type.startsWith('image/'))
    setFiles((prevFiles) => [...prevFiles, ...imageFiles]);
  }
  return (
    <div className="flex justify-center">
      <form
        className="w-3/4 h-72 border-2 rounded-2xl border-dashed border-fuchsia-500 md:w-2/4 md:h-96"
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
        <label htmlFor="browse" className="flex h-full" />
      </form>
      <div className="mt-4 flex flex-wrap gap-4">
        {files.map((file, index) => (
          <div
            key={index}
            className="w-32 h-32 overflow-hidden border rounded-md"
          >
            <img
              src={URL.createObjectURL(file)}
              alt={`preview-${index}`}
              className="w-full h-full object-cover"
            />
          </div>
        ))}
      </div>
    </div>
  );
}
