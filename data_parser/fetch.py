import sqlite3

VERSIONS = ['kjv', 'asv']
DB_PATH = 'sql/bible-sqlite.db'  # Update this path

conn = sqlite3.connect(DB_PATH)
cursor = conn.cursor()

BOOK_NAMES = [
    "", "Genesis", "Exodus", "Leviticus", "Numbers", "Deuteronomy",
    "Joshua", "Judges", "Ruth", "1 Samuel", "2 Samuel", "1 Kings",
    "2 Kings", "1 Chronicles", "2 Chronicles", "Ezra", "Nehemiah",
    "Esther", "Job", "Psalms", "Proverbs", "Ecclesiastes", "Song of Solomon",
    "Isaiah", "Jeremiah", "Lamentations", "Ezekiel", "Daniel", "Hosea",
    "Joel", "Amos", "Obadiah", "Jonah", "Micah", "Nahum", "Habakkuk",
    "Zephaniah", "Haggai", "Zechariah", "Malachi", "Matthew", "Mark",
    "Luke", "John", "Acts", "Romans", "1 Corinthians", "2 Corinthians",
    "Galatians", "Ephesians", "Philippians", "Colossians", "1 Thessalonians",
    "2 Thessalonians", "1 Timothy", "2 Timothy", "Titus", "Philemon",
    "Hebrews", "James", "1 Peter", "2 Peter", "1 John", "2 John",
    "3 John", "Jude", "Revelation"
]

BOOK_COUNT = 66  # Number of books in the Bible (adjust if needed)
CHAPTER_COUNT = 150  # Maximum chapters in any book (adjust if needed)
VERSE_COUNT = 176  # Maximum verses in any chapter (adjust if needed)

def get_verse_id(book_num, chapter_num, verse_num):
    return f'{book_num:02d}{chapter_num:03d}{verse_num:03d}'

def is_valid_id(verse_id, version):
    query = f"SELECT 1 FROM t_{version} WHERE id = ? LIMIT 1"
    cursor.execute(query, (verse_id,))
    return cursor.fetchone() is not None


# Fetch verse based on book, chapter, and verse
def fetch_verse(verse_id, version):
    query = f"SELECT * FROM t_{version} WHERE id = ?"
    cursor.execute(query, (verse_id,))
    result = cursor.fetchone()

    if result:

        return result  # Assuming the verse is in the 'result' field
    else:
        return None


def write_verse_to_file(book_num, chapter_num, verse_num, version, file):
    write_verse_to_file_from_id(get_verse_id(book_num, chapter_num, verse_num), version, file)

def write_verse_to_file_from_id(verse_id, version, file):
    verse = fetch_verse(verse_id, version)
    if verse:
        verse_id, book_num, chapter_num, verse_num, text = verse
        # Construct the verse reference (e.g., John 3:16)
        verse_ref = f"{BOOK_NAMES[book_num]} {chapter_num}:{verse_num}"
        file.write(f"{verse_ref} - {text} ({version.upper()})\n")  # Adjust the format as needed


for version in VERSIONS:
    # Clear the output file before writing new content
    file_path = f'../data/{version}.txt'
    with open(file_path, 'w') as f:
        f.write('')

    with open(file_path, 'a') as f:
        for book_num in range(1, BOOK_COUNT + 1):
            valid_ids = set()
            for chapter_num in range(1, CHAPTER_COUNT + 1):
                for verse_num in range(1, VERSE_COUNT + 1):
                    verse_id = get_verse_id(book_num, chapter_num, verse_num)
                    if is_valid_id(verse_id, version):
                        valid_ids.add(verse_id)

            f.write(f"%\"{BOOK_NAMES[book_num]}\":{len(valid_ids)}\n")
            for verse_id in sorted(valid_ids):
                write_verse_to_file_from_id(verse_id, version, f)
            print(f"{version}: {BOOK_NAMES[book_num]} DONE")


# Close the database connection when done
conn.close()