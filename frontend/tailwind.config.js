/** @type {import('tailwindcss').Config} */
module.exports = {
  content: [
   "./src/**/*.{js,jsx}",
  ],
  theme: {
    extend: {
      backgroundImage: {
        'hero': "url('./img/herobackground.webp')",
      }
    },
  },
  plugins: [],
}

