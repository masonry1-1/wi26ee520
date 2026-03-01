#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <map>

using json = nlohmann::json;

/* ---------------- HTTP ---------------- */

static size_t write_cb(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t total = size * nmemb;
    std::string* s = static_cast<std::string*>(userp);
    s->append((char*)contents, total);
    return total;
}

std::string post_json(const std::string& body, const std::string& key) {
    CURL* curl = curl_easy_init();
    std::string response;

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, ("Authorization: Bearer " + key).c_str());

    curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/responses");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    curl_easy_perform(curl);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    return response;
}

/* ---------------- Tools ---------------- */

json calculator(json args) {
    std::string op = args["op"];
    double a = args["a"];
    double b = args["b"];

    if (op == "add") return {{"result", a + b}};
    if (op == "sub") return {{"result", a - b}};
    if (op == "mul") return {{"result", a * b}};
    if (op == "div") return {{"result", a / b}};

    return {{"error", "invalid operation"}};
}

json statistics(json args) {
    std::vector<double> vals = args["values"];

    double mean = std::accumulate(vals.begin(), vals.end(), 0.0) / vals.size();

    double var = 0;
    for (double x : vals) var += (x - mean) * (x - mean);
    var /= vals.size();
    double stddev = std::sqrt(var);

    std::sort(vals.begin(), vals.end());
    double median = vals.size() % 2
        ? vals[vals.size()/2]
        : (vals[vals.size()/2 - 1] + vals[vals.size()/2]) / 2.0;

    return {
        {"mean", mean},
        {"stddev", stddev},
        {"median", median}
    };
}

json fibonacci(json args) {
    int n = args["n"];
    std::vector<long long> seq;
    long long a = 0, b = 1;
    for (int i = 0; i < n; i++) {
        seq.push_back(a);
        long long next = a + b;
        a = b;
        b = next;
    }
    return {{"sequence", seq}};
}

/* ---------------- Main ---------------- */

int main(int argc, char** argv) {

    const char* key = getenv("OPENAI_API_KEY");
    if (!key) {
        std::cerr << "Set OPENAI_API_KEY first.\n";
        return 1;
    }

    std::string query;
    if (argc >= 2) query = argv[1];
    else std::getline(std::cin, query);

    json request = {
        {"model", "gpt-5-nano"},
        {"input", query}
    };

    std::string response = post_json(request.dump(), key);

    std::cout << response << std::endl;
}
