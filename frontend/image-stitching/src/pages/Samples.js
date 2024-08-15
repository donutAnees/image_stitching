import Hero from "../components/Hero";

const downloadLinks = [
  { url: "https://example.com/sample1", label: "Sample 1" },
  { url: "https://example.com/sample2", label: "Sample 2" },
  { url: "https://example.com/sample3", label: "Sample 3" },
];

export default function Samples() {
  return (
    <Hero>
      <div className="pt-20 pl-10">
        <div className="text-white flex flex-col">
          {downloadLinks.map((link, index) => (
            <a
              key={index}
              href={link.url}
              target="_blank"
              rel="noopener noreferrer"
              className="download-link"
            >
              {link.label}
            </a>
          ))}
        </div>
      </div>
    </Hero>
  );
}
