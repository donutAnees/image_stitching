import { Link } from "react-router-dom";
const item = ["samples"];

export default function Navlist() {
  return (
    <div className="flex items-center">
      <ul className="flex">
        {item.map((i, index) => {
          return (
            <Link key={index} to={`/${i}`} className="px-1.5 md:px-6 font-medium text-l hover:text-sky-500">
              {i}
            </Link>
          );
        })}
      </ul>
    </div>
  );
}