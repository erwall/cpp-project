#include "article.h"

Article::Article(std::string title, std::string author, std::string text,
		int id)
{
	this->title = title;
	this->author = author;
	this->text = text;
	this->id = id;
}
std::string Article::getTitle() {
	return title;
}
std::string Article::getAuthor() {
	return author;
}
std::string Article::getText() {
	return text;
}
int Article::getID() {
	return id;

}